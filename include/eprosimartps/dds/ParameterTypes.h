/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file ParameterTypes.h
 *
 *  Created on: Mar 20, 2014
 *      Author: Gonzalo Rodriguez Canosa
 *      email:  gonzalorodriguez@eprosima.com
 *              grcanosa@gmail.com  	
 */

#ifndef PARAMETERTYPES_H_
#define PARAMETERTYPES_H_


namespace eprosima {
namespace dds {



enum ParameterId_t :uint16_t
{
 PID_PAD =0x0000,
 PID_SENTINEL =0x0001,
 //PID_USER_DATA =0x002c,
 PID_TOPIC_NAME =0x0005,
 PID_TYPE_NAME =0x0007,
// PID_GROUP_DATA =0x002d,
// PID_TOPIC_DATA =0x002e,
// PID_DURABILITY =0x001d,
// PID_DURABILITY_SERVICE =0x001e,
// PID_DEADLINE =0x0023,
// PID_LATENCY_BUDGET =0x0027,
// PID_LIVELINESS =0x001b,
// PID_RELIABILITY =0x001A,
// PID_LIFESPAN =0x002b,
// PID_DESTINATION_ORDER =0x0025,
// PID_HISTORY =0x0040,
// PID_RESOURCE_LIMITS =0x0041,
// PID_OWNERSHIP =0x001f,
// PID_OWNERSHIP_STRENGTH =0x0006,
// PID_PRESENTATION =0x0021,
// PID_PARTITION =0x0029,
// PID_TIME_BASED_FILTER =0x0004,
// PID_TRANSPORT_PRIORITY =0x0049,
// PID_PROTOCOL_VERSION =0x0015,
// PID_VENDORID =0x0016,
 PID_UNICAST_LOCATOR =0x002f,
 PID_MULTICAST_LOCATOR =0x0030,
 //PID_MULTICAST_IPADDRESS =0x0011,
 PID_DEFAULT_UNICAST_LOCATOR =0x0031,
 PID_DEFAULT_MULTICAST_LOCATOR =0x0048,
 PID_METATRAFFIC_UNICAST_LOCATOR =0x0032,
 PID_METATRAFFIC_MULTICAST_LOCATOR =0x0033,
 //PID_DEFAULT_UNICAST_IPADDRESS =0x000c,
 PID_DEFAULT_UNICAST_PORT =0x000e,
 //PID_METATRAFFIC_UNICAST_IPADDRESS =0x0045,
 PID_METATRAFFIC_UNICAST_PORT =0x000d,
// PID_METATRAFFIC_MULTICAST_IPADDRESS =0x000b,
 PID_METATRAFFIC_MULTICAST_PORT =0x0046,
// PID_EXPECTS_INLINE_QOS =0x0043,
// PID_PARTICIPANT_MANUAL_LIVELINESS_COUNT =0x0034,
// PID_PARTICIPANT_BUILTIN_ENDPOINTS =0x0044,
// PID_PARTICIPANT_LEASE_DURATION =0x0002,
// PID_CONTENT_FILTER_PROPERTY =0x0035,
// PID_PARTICIPANT_GUID =0x0050,
// PID_PARTICIPANT_ENTITYID =0x0051,
// PID_GROUP_GUID =0x0052,
// PID_GROUP_ENTITYID =0x0053,
// PID_BUILTIN_ENDPOINT_SET =0x0058,
// PID_PROPERTY_LIST =0x0059,
// PID_TYPE_MAX_SIZE_SERIALIZED =0x0060,
// PID_ENTITY_NAME =0x0062,
 PID_KEY_HASH =0x0070,
 PID_STATUS_INFO =0x0071
};




//!Base Parameter class with parameter PID and parameter length in bytes.
class Parameter_t{
public:
	ParameterId_t Pid;
	uint16_t length;
	Parameter_t(){};
	Parameter_t(Parameter_t* P):Pid(P->Pid),length(P->length){};
};

class ParameterLocator_t:public Parameter_t{
public:
	Locator_t locator;
	ParameterLocator_t(){};
	ParameterLocator_t(Parameter_t* P):Parameter_t(P){};
};

class ParameterString_t:public Parameter_t{
public:
	std::string p_str;
	ParameterString_t(){};
	ParameterString_t(Parameter_t* P):Parameter_t(P){};
};

class ParameterPort_t:public Parameter_t{
public:
	uint32_t port;
	ParameterPort_t(){};
	ParameterPort_t(Parameter_t* P):Parameter_t(P){};
};



}//end of namespace dds
}//end of namespace rtps



#endif /* PARAMETERTYPES_H_ */
