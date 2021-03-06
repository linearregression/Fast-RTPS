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

#ifndef MOCK_TRANSPORT_H
#define MOCK_TRANSPORT_H

#include <fastrtps/transport/TransportInterface.h>
#include <utility>
#include <vector>

namespace eprosima{
namespace fastrtps{
namespace rtps{

typedef struct  
{
    int maximumChannels;
    int supportedKind;
} MockTransportDescriptor;

class MockTransport: public TransportInterface
{
    public:

        MockTransport(const MockTransportDescriptor& descriptor);

        MockTransport();

        ~MockTransport();

        bool init();

        //API implementation
        virtual bool IsOutputChannelOpen(const Locator_t&) const;
        virtual bool IsInputChannelOpen(const Locator_t&)  const;

        virtual bool OpenOutputChannel(const Locator_t&); 
        virtual bool OpenInputChannel(const Locator_t&); 

        virtual bool CloseOutputChannel(const Locator_t&);
        virtual bool CloseInputChannel(const Locator_t&);

        virtual Locator_t RemoteToMainLocal(const Locator_t&) const;

        virtual bool IsLocatorSupported(const Locator_t&)  const;
        virtual bool DoLocatorsMatch(const Locator_t&, const Locator_t&) const;

        virtual bool Send(const octet* sendBuffer, uint32_t sendBufferSize, const Locator_t& localLocator, const Locator_t& remoteLocator);
        virtual bool Receive(octet* receiveBuffer, uint32_t receiveBufferCapacity, uint32_t& receiveBufferSize,
                const Locator_t& localLocator, Locator_t& remoteLocator);

        virtual LocatorList_t NormalizeLocator(const Locator_t& locator);

        //Helpers and message record
        typedef struct
        {
            Locator_t destination;
            Locator_t origin;
            std::vector<octet> data;
        } MockMessage;

        std::vector<MockMessage> mockMessagesToReceive;
        std::vector<MockMessage> mockMessagesSent;

        // For the mock, port + direction tuples will have a 1:1 relatonship with channels

        typedef uint32_t Port;
        std::vector<Port> mockOpenOutputChannels;
        std::vector<Port> mockOpenInputChannels;

        const static int DefaultKind = 1;
        int mockSupportedKind;

        const static int DefaultMaxChannels = 10;
        int mockMaximumChannels;

        //Helper persistent handles
        static std::vector<MockTransport*> mockTransportInstances;
};

} // namespace rtps
} // namespace fastrtps
} // namespace eprosima

#endif
