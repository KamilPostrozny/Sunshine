/**
 * @file src/latency_protocol.h
 * @brief Declarations for negotiating the client's latency protocol (fork extension).
 *
 * The latency protocol carries clock sync and frame-time corrections between a client and this
 * host as opaque payloads of one encrypted control-stream message type. The host advertises it in
 * `x-ss-general.featureFlags` of its DESCRIBE reply when built with it, and a session speaks it only
 * when the client sets the matching bit in `x-ml-general.featureFlags` of its ANNOUNCE, so other
 * clients see a normal Sunshine.
 */
#pragma once

// standard includes
#include <cstdint>

#ifndef SUNSHINE_LATENCY_PROTOCOL
  #define SUNSHINE_LATENCY_PROTOCOL 0
#endif

namespace latency_protocol {
  /**
   * @brief Whether this build speaks the latency protocol (`SUNSHINE_ENABLE_LATENCY_PROTOCOL`).
   */
  constexpr bool built = SUNSHINE_LATENCY_PROTOCOL != 0;

  /**
   * @brief Host bit in `x-ss-general.featureFlags` (moonlight-common-c's `LI_FF_LATENCY_PROTOCOL`).
   */
  constexpr std::uint32_t host_feature_flag = 0x40000000;

  /**
   * @brief Client bit in `x-ml-general.featureFlags` (moonlight-common-c's `ML_FF_LATENCY_PROTOCOL`).
   */
  constexpr std::uint32_t client_feature_flag = 0x40000000;

  /**
   * @brief Control-stream message type carrying a latency protocol payload (Sunshine extension range).
   */
  constexpr std::uint16_t message_type = 0x55f0;

  /**
   * @brief Add the latency protocol to the host feature flags of a DESCRIBE reply.
   *
   * @param feature_flags The platform's feature flags.
   * @param is_built Whether the build speaks the protocol.
   * @return `feature_flags`, with `host_feature_flag` set when `is_built`.
   */
  constexpr std::uint32_t advertise(std::uint32_t feature_flags, bool is_built = built) {
    return is_built ? feature_flags | host_feature_flag : feature_flags;
  }

  /**
   * @brief Decide whether a session speaks the latency protocol.
   *
   * @param ml_feature_flags The client's `x-ml-general.featureFlags` from its ANNOUNCE.
   * @param is_built Whether the build speaks the protocol.
   * @return True when the build speaks it and the client offered it.
   */
  constexpr bool negotiated(std::uint32_t ml_feature_flags, bool is_built = built) {
    return is_built && (ml_feature_flags & client_feature_flag) != 0;
  }
}  // namespace latency_protocol
