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
 * @file ReqRepHelloWorldRequester.hpp
 *
 */

#ifndef _TEST_BLACKBOX_REQREPHELLOWORLDREQUESTER_HPP_
#define _TEST_BLACKBOX_REQREPHELLOWORLDREQUESTER_HPP_

#include "types/HelloWorldType.h"

#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/subscriber/SubscriberListener.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/publisher/PublisherListener.h>
#include <fastrtps/attributes/PublisherAttributes.h>

#include <list>
#include <condition_variable>

using namespace eprosima::fastrtps;

class ReqRepHelloWorldRequester 
{
    public:

        class ReplyListener: public SubscriberListener
        {
            public:
                ReplyListener(ReqRepHelloWorldRequester &requester) : requester_(requester) {};
                ~ReplyListener(){};
                void onNewDataMessage(Subscriber *sub);
                void onSubscriptionMatched(Subscriber* /*sub*/, MatchingInfo& info)
                {
                    if (info.status == MATCHED_MATCHING)
                        requester_.matched();
                }

            private:

                ReplyListener& operator=(const ReplyListener&) NON_COPYABLE_CXX11;

                ReqRepHelloWorldRequester &requester_;
        } reply_listener_;

        class RequestListener : public PublisherListener
    {
        public:

            RequestListener(ReqRepHelloWorldRequester &requester) : requester_(requester){};
            ~RequestListener(){};
            void onPublicationMatched(Publisher* /*pub*/, MatchingInfo &info)
            {
                if (info.status == MATCHED_MATCHING)
                    requester_.matched();
            }

        private:

            RequestListener& operator=(const RequestListener&) NON_COPYABLE_CXX11;

            ReqRepHelloWorldRequester &requester_;

    } request_listener_;

        ReqRepHelloWorldRequester();
        virtual ~ReqRepHelloWorldRequester();
        void init();
        bool isInitialized() const { return initialized_; }
        void newNumber(SampleIdentity related_sample_identity, uint16_t number);
        void block(const std::chrono::seconds &seconds);
        void waitDiscovery();
        void matched();
        void send(const uint16_t number);
        virtual void configSubscriber(const std::string& suffix) = 0;
        virtual void configPublisher(const std::string& suffix) = 0;

    protected:
	PublisherAttributes puattr;
        SubscriberAttributes sattr;
    private:

        ReqRepHelloWorldRequester& operator=(const ReqRepHelloWorldRequester&)NON_COPYABLE_CXX11;

        uint16_t current_number_;
        uint16_t number_received_;
        Participant *participant_;
        Subscriber *reply_subscriber_;
	Publisher *request_publisher_;
        bool initialized_;
        std::mutex mutex_;
        std::condition_variable cv_;
        std::mutex mutexDiscovery_;
        std::condition_variable cvDiscovery_;
        unsigned int matched_;
        HelloWorldType type_;
        SampleIdentity related_sample_identity_;
};

#endif // _TEST_BLACKBOX_REQREPHELLOWORLDREQUESTER_HPP_
