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
 * @file ReqRepAsReliableHelloWorldReplier.hpp
 *
 */

#ifndef _TEST_BLACKBOX_REQREPASRELIABLEHELLOWORLDREPLIER_HPP_
#define _TEST_BLACKBOX_REQREPASRELIABLEHELLOWORLDREPLIER_HPP_

#include "ReqRepHelloWorldReplier.hpp" 
#include <boost/asio.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>

class ReqRepAsReliableHelloWorldReplier : public ReqRepHelloWorldReplier
{
    public:
        void configSubscriber(const std::string& suffix)
        {
            sattr.qos.m_reliability.kind = eprosima::fastrtps::RELIABLE_RELIABILITY_QOS;

            std::ostringstream t;

            t << "ReqRepAsReliableHelloworld_" << boost::asio::ip::host_name() << "_" << boost::interprocess::ipcdetail::get_current_process_id() << "_" << suffix;

            sattr.topic.topicName = t.str();
        };

        void configPublisher(const std::string& suffix)
        {
            puattr.qos.m_reliability.kind = RELIABLE_RELIABILITY_QOS;

            std::ostringstream t;

            t << "ReqRepAsReliableHelloworld_" << boost::asio::ip::host_name() << "_" << boost::interprocess::ipcdetail::get_current_process_id() << "_" << suffix;

            puattr.topic.topicName = t.str();
        }

        ReqRepAsReliableHelloWorldReplier& memoryMode(eprosima::fastrtps::rtps::MemoryManagementPolicy_t memoryMode){
            sattr.historyMemoryPolicy=memoryMode;
            puattr.historyMemoryPolicy=memoryMode;
            return *this;
        }
};

#endif // _TEST_BLACKBOX_REQREPASRELIABLEHELLOWORLDREPLIER_HPP_

