/**
 * @file tests/unit/test_latency_protocol.cpp
 * @brief Test negotiating the client's latency protocol (fork extension).
 */

// test includes
#include "../tests_common.h"

// local includes
#include <src/latency_protocol.h>

/**
 * @brief A build with the protocol adds its bit to the platform's flags; a build without it leaves them alone.
 */
TEST(LatencyProtocolTest, AdvertisesOnlyWhenBuiltWithIt) {
  for (const std::uint32_t platform_flags : {0x0U, 0x1U, 0x3U}) {
    EXPECT_EQ(latency_protocol::advertise(platform_flags, true), platform_flags | latency_protocol::host_feature_flag);
    EXPECT_EQ(latency_protocol::advertise(platform_flags, false), platform_flags);
  }
}

/**
 * @brief A session speaks the protocol only when the build has it and the client offered it.
 */
TEST(LatencyProtocolTest, NegotiatedOnlyWhenBuiltWithItAndOffered) {
  // Stock Moonlight sends ML_FF_FEC_STATUS | ML_FF_SESSION_ID_V1.
  constexpr std::uint32_t stock_client = 0x3;
  constexpr std::uint32_t offering_client = stock_client | latency_protocol::client_feature_flag;

  EXPECT_TRUE(latency_protocol::negotiated(offering_client, true));
  EXPECT_FALSE(latency_protocol::negotiated(stock_client, true));
  EXPECT_FALSE(latency_protocol::negotiated(0, true));
  EXPECT_FALSE(latency_protocol::negotiated(offering_client, false));
}

/**
 * @brief The protocol's wire values match moonlight-common-c's fork and stay clear of upstream's ranges.
 */
TEST(LatencyProtocolTest, WireValuesMatchTheClient) {
  EXPECT_EQ(latency_protocol::host_feature_flag, 0x40000000U);
  EXPECT_EQ(latency_protocol::client_feature_flag, 0x40000000U);
  EXPECT_EQ(latency_protocol::message_type, 0x55f0);
  EXPECT_EQ(latency_protocol::message_type & 0xff00, 0x5500);
}
