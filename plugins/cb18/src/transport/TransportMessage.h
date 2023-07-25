/*
 * TransportMessage.h
 *
 *  Created on: Jun 15, 2021
 *      Author: ubuntu
 */

#ifndef SERVER_NODES_HMI_SERVICES_TRANSPORT_TRANSPORTMESSAGE_H_
#define SERVER_NODES_HMI_SERVICES_TRANSPORT_TRANSPORTMESSAGE_H_

#include "../../protocol/generated/public_generated.h"
#include "../../protocol/generated/report_generated.h"
#include "../../protocol/generated/request_generated.h"
#include "../../protocol/generated/response_generated.h"

namespace swr {

namespace transport {

using RequestBuffer = const swr::Request;

using ResponseBuffer = flatbuffers::FlatBufferBuilder;

using ReportBuffer = flatbuffers::FlatBufferBuilder;

using MessageId = std::uint16_t;

}  // namespace transport

}  // namespace swr

#endif /* SERVER_NODES_HMI_SERVICES_TRANSPORT_TRANSPORTMESSAGE_H_ */
