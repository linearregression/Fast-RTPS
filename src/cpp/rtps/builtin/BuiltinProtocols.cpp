// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file BuiltinProtocols.cpp
 *
 */

#include <fastrtps/rtps/builtin/BuiltinProtocols.h>
#include <fastrtps/rtps/common/Locator.h>

#include <fastrtps/rtps/builtin/discovery/participant/PDPSimple.h>
#include <fastrtps/rtps/builtin/discovery/endpoint/EDP.h>

#include <fastrtps/rtps/builtin/liveliness/WLP.h>

#include "../participant/RTPSParticipantImpl.h"


#include <fastrtps/log/Log.h>
#include <fastrtps/utils/IPFinder.h>



using namespace eprosima::fastrtps;

namespace eprosima {
namespace fastrtps{
namespace rtps {


BuiltinProtocols::BuiltinProtocols():
				mp_participantImpl(nullptr),
				mp_PDP(nullptr),
				mp_WLP(nullptr),
				m_SPDP_WELL_KNOWN_MULTICAST_PORT(7400),
				m_SPDP_WELL_KNOWN_UNICAST_PORT(7410)
{
	// TODO Auto-generated constructor stub
	m_useMandatory = false;
}

BuiltinProtocols::~BuiltinProtocols() {
    // Send participant is disposed
    if(mp_PDP != nullptr)
        mp_PDP->announceParticipantState(true, true);
	// TODO Auto-generated destructor stub
	if(mp_WLP!=nullptr)
		delete(mp_WLP);
	if(mp_PDP!=nullptr)
		delete(mp_PDP);
}


bool BuiltinProtocols::initBuiltinProtocols(RTPSParticipantImpl* p_part, BuiltinAttributes& attributes)
{
	mp_participantImpl = p_part;
	m_att = attributes;

	m_SPDP_WELL_KNOWN_MULTICAST_PORT = mp_participantImpl->getAttributes().port.getMulticastPort(m_att.domainId);

	m_SPDP_WELL_KNOWN_UNICAST_PORT =
			mp_participantImpl->getAttributes().port.getUnicastPort(m_att.domainId,mp_participantImpl->getAttributes().participantID);

	/* If metatrafficMulticastLocatorList is empty, add mandatory default Locators
	   Else -> Take them */

	/* INSERT DEFAULT MANDATORY MULTICAST LOCATORS HERE */

	//UDPv4
	this->m_mandatoryMulticastLocator.kind = LOCATOR_KIND_UDPv4;
	m_mandatoryMulticastLocator.port = m_SPDP_WELL_KNOWN_MULTICAST_PORT;
	m_mandatoryMulticastLocator.set_IP4_address(239,255,0,1);
	if(m_att.metatrafficMulticastLocatorList.empty())
	{
		m_metatrafficMulticastLocatorList.push_back(m_mandatoryMulticastLocator);
	}
	else
	{
		//Copy metatrafficMulticastLocatorList from the BuiltinAttributs
		m_useMandatory = false;
		for(std::vector<Locator_t>::iterator it = m_att.metatrafficMulticastLocatorList.begin();
				it!=m_att.metatrafficMulticastLocatorList.end();++it)
		{
			m_metatrafficMulticastLocatorList.push_back(*it);
		}
	}
	//Create ReceiverResources now and update the list with the REAL used ones
	p_part->createReceiverResources(m_metatrafficMulticastLocatorList, true);
	/* INSERT DEFAULT UNICAST LOCATORS HERE */

	if(m_att.metatrafficUnicastLocatorList.empty())
	{
		//Add default metatrafficUnicastLocators
		LocatorList_t locators;
		IPFinder::getIP4Address(&locators);
        for(std::vector<Locator_t>::iterator it=locators.begin();it!=locators.end();++it)
        {
            it->port = m_SPDP_WELL_KNOWN_UNICAST_PORT;
            m_metatrafficUnicastLocatorList.push_back(*it);
        }
	}
	else
	{
		//If locators existed, just import them to the class
		for(std::vector<Locator_t>::iterator it = m_att.metatrafficUnicastLocatorList.begin();
				it!=m_att.metatrafficUnicastLocatorList.end();++it)
		{
			m_metatrafficUnicastLocatorList.push_back(*it);
		}
	}
	//Create ReceiverResources now and update the list with the REAL used ones
	p_part->createReceiverResources(m_metatrafficUnicastLocatorList, true);

	/* 
	In principle there is no need to create Senders now.  When a builtin Writer/Reader is created,
	it will be applied to the DefaultOutLocatorList (already created).
	*/

	if(m_att.use_SIMPLE_RTPSParticipantDiscoveryProtocol)
	{
		mp_PDP = new PDPSimple(this);
		mp_PDP->initPDP(mp_participantImpl);
		if(m_att.use_WriterLivelinessProtocol)
		{
			mp_WLP = new WLP(this);
			mp_WLP->initWL(mp_participantImpl);
		}
		mp_PDP->announceParticipantState(true);
		mp_PDP->resetParticipantAnnouncement();
	}

	return true;
}

bool BuiltinProtocols::updateMetatrafficLocators(LocatorList_t& loclist)
{
	m_metatrafficUnicastLocatorList = loclist;
	return true;
}

bool BuiltinProtocols::addLocalWriter(RTPSWriter* w,fastrtps::TopicAttributes& topicAtt,fastrtps::WriterQos& wqos)
{
	bool ok = false;
	if(mp_PDP!=nullptr)
	{
		ok |= mp_PDP->getEDP()->newLocalWriterProxyData(w,topicAtt,wqos);
	}
	else
	{
		logWarning(RTPS_EDP, "EDP is not used in this Participant, register a Writer is impossible");
	}
	if(mp_WLP !=nullptr)
	{
		ok|= mp_WLP->addLocalWriter(w,wqos);
	}
	else
	{
		logWarning(RTPS_LIVELINESS, "LIVELINESS is not used in this Participant, register a Writer is impossible");
	}
	return ok;
}

bool BuiltinProtocols::addLocalReader(RTPSReader* R,fastrtps::TopicAttributes& topicAtt, fastrtps::ReaderQos& rqos)
{
	bool ok = false;
	if(mp_PDP!=nullptr)
	{
		ok |= mp_PDP->getEDP()->newLocalReaderProxyData(R,topicAtt, rqos);
	}
	else
	{
		logWarning(RTPS_EDP, "EDP is not used in this Participant, register a Reader is impossible");
	}
	return ok;
}

bool BuiltinProtocols::updateLocalWriter(RTPSWriter* W,WriterQos& wqos)
{
	bool ok = false;
	if(mp_PDP!=nullptr && mp_PDP->getEDP()!=nullptr)
	{
		ok |= mp_PDP->getEDP()->updatedLocalWriter(W,wqos);
	}
	if(mp_WLP!=nullptr)
	{
		ok |= mp_WLP->updateLocalWriter(W,wqos);
	}
	return ok;
}

bool BuiltinProtocols::updateLocalReader(RTPSReader* R,ReaderQos& rqos)
{
	bool ok = false;
	if(mp_PDP!=nullptr && mp_PDP->getEDP()!=nullptr)
	{
		ok |= mp_PDP->getEDP()->updatedLocalReader(R,rqos);
	}
	return ok;
}

bool BuiltinProtocols::removeLocalWriter(RTPSWriter* W)
{
	bool ok = false;
	if(mp_WLP !=nullptr)
	{
		ok|= mp_WLP->removeLocalWriter(W);
	}
	if(mp_PDP!=nullptr && mp_PDP->getEDP() != nullptr)
	{
		ok|= mp_PDP->getEDP()->removeLocalWriter(W);
	}
	return ok;
}

bool BuiltinProtocols::removeLocalReader(RTPSReader* R)
{
	bool ok = false;
	if(mp_PDP!=nullptr && mp_PDP->getEDP() != nullptr)
	{
		ok|= mp_PDP->getEDP()->removeLocalReader(R);
	}
	return ok;
}

void BuiltinProtocols::announceRTPSParticipantState()
{
	mp_PDP->announceParticipantState(false);
}

void BuiltinProtocols::stopRTPSParticipantAnnouncement()
{
	mp_PDP->stopParticipantAnnouncement();
}

void BuiltinProtocols::resetRTPSParticipantAnnouncement()
{
	mp_PDP->resetParticipantAnnouncement();
}

}
} /* namespace rtps */
} /* namespace eprosima */
