/**
 * @file aiscpp_metadata.hpp
 * @author Brendan Knapp (brendan.g.knapp@gmail.com)
 * @brief
 * @version 0.1.1
 * @date 2021-01-17
 *
 * @copyright Copyright (c) 2021
 *
 */


#ifndef AISCPP_METADATA_HPP_
#define AISCPP_METADATA_HPP_


#include <array>
#include <limits>
#include <string_view>
#include <tuple>
#include <charconv>

namespace aiscpp {

/**
 * Lookup tables
 *
 */
namespace tbls {
/**
 * @brief __Navigation Status__ descriptions.
 *
 */
constexpr std::array nav_status = {
    "Under way using engine",                                       // 0
    "At anchor",                                                    // 1
    "Not under command",                                            // 2
    "Restricted manoeuverability",                                  // 3
    "Constrained by her draught",                                   // 4
    "Moored",                                                       // 5
    "Aground",                                                      // 6
    "Engaged in Fishing",                                           // 7
    "Under way sailing",                                            // 8
    "Reserved for future amendment of Navigational Status for HSC", // 9
    "Reserved for future amendment of Navigational Status for WIG", // 10
    "Reserved for future use",                                      // 11
    "Reserved for future use",                                      // 12
    "Reserved for future use",                                      // 13
    "AIS-SART is active",                                           // 14
    "Not defined (default)",                                        // 15
};


constexpr std::array ship_type = {
    "Not available (default)",                             // 0
    "Reserved for future use",                             // 1
    "Reserved for future use",                             // 2
    "Reserved for future use",                             // 3
    "Reserved for future use",                             // 4
    "Reserved for future use",                             // 5
    "Reserved for future use",                             // 6
    "Reserved for future use",                             // 7
    "Reserved for future use",                             // 8
    "Reserved for future use",                             // 9
    "Reserved for future use",                             // 10
    "Reserved for future use",                             // 11
    "Reserved for future use",                             // 12
    "Reserved for future use",                             // 13
    "Reserved for future use",                             // 14
    "Reserved for future use",                             // 15
    "Reserved for future use",                             // 16
    "Reserved for future use",                             // 17
    "Reserved for future use",                             // 18
    "Reserved for future use",                             // 19
    "Wing in ground (WIG), all ships of this type"         // 20
    "Wing in ground (WIG), Hazardous category A",          // 21
    "Wing in ground (WIG), Hazardous category B",          // 22
    "Wing in ground (WIG), Hazardous category C",          // 23
    "Wing in ground (WIG), Hazardous category D",          // 24
    "Wing in ground (WIG), Reserved for future use",       // 25
    "Wing in ground (WIG), Reserved for future use",       // 26
    "Wing in ground (WIG), Reserved for future use",       // 27
    "Wing in ground (WIG), Reserved for future use",       // 28
    "Wing in ground (WIG), Reserved for future use",       // 29
    "Fishing",                                             // 30
    "Towing",                                              // 31
    "Towing: length exceeds 200m or breadth exceeds 25m",  // 32
    "Dredging or underwater ops",                          // 33
    "Diving ops",                                          // 34
    "Military ops",                                        // 35
    "Sailing",                                             // 36
    "Pleasure Craft",                                      // 37
    "Reserved",                                            // 38
    "Reserved",                                            // 39
    "High speed craft (HSC), all ships of this type",      // 40
    "High speed craft (HSC), Hazardous category A",        // 41
    "High speed craft (HSC), Hazardous category B",        // 42
    "High speed craft (HSC), Hazardous category C",        // 43
    "High speed craft (HSC), Hazardous category D",        // 44
    "High speed craft (HSC), Reserved for future use",     // 45
    "High speed craft (HSC), Reserved for future use",     // 46
    "High speed craft (HSC), Reserved for future use",     // 47
    "High speed craft (HSC), Reserved for future use",     // 48
    "High speed craft (HSC), No additional information",   // 49
    "Pilot Vessel",                                        // 50
    "Search and Rescue vessel",                            // 51
    "Tug",                                                 // 52
    "Port Tender",                                         // 53
    "Anti-pollution equipment",                            // 54
    "Law Enforcement",                                     // 55
    "Spare - Local Vessel",                                // 56
    "Spare - Local Vessel",                                // 57
    "Medical Transport",                                   // 58
    "Noncombatant ship according to RR Resolution No. 18", // 59
    "Passenger, all ships of this type",                   // 60
    "Passenger, Hazardous category A",                     // 61
    "Passenger, Hazardous category B",                     // 62
    "Passenger, Hazardous category C",                     // 63
    "Passenger, Hazardous category D",                     // 64
    "Passenger, Reserved for future use",                  // 65
    "Passenger, Reserved for future use",                  // 66
    "Passenger, Reserved for future use",                  // 67
    "Passenger, Reserved for future use",                  // 68
    "Passenger, No additional information",                // 69
    "Cargo, all ships of this type",                       // 70
    "Cargo, Hazardous category A",                         // 71
    "Cargo, Hazardous category B",                         // 72
    "Cargo, Hazardous category C",                         // 73
    "Cargo, Hazardous category D",                         // 74
    "Cargo, Reserved for future use",                      // 75
    "Cargo, Reserved for future use",                      // 76
    "Cargo, Reserved for future use",                      // 77
    "Cargo, Reserved for future use",                      // 78
    "Cargo, No additional information",                    // 79
    "Tanker, all ships of this type",                      // 80
    "Tanker, Hazardous category A",                        // 81
    "Tanker, Hazardous category B",                        // 82
    "Tanker, Hazardous category C",                        // 83
    "Tanker, Hazardous category D",                        // 84
    "Tanker, Reserved for future use",                     // 85
    "Tanker, Reserved for future use",                     // 86
    "Tanker, Reserved for future use",                     // 87
    "Tanker, Reserved for future use",                     // 88
    "Tanker, No additional information",                   // 89
    "Other Type, all ships of this type",                  // 90
    "Other Type, Hazardous category A",                    // 91
    "Other Type, Hazardous category B",                    // 92
    "Other Type, Hazardous category C",                    // 93
    "Other Type, Hazardous category D",                    // 94
    "Other Type, Reserved for future use",                 // 95
    "Other Type, Reserved for future use",                 // 96
    "Other Type, Reserved for future use",                 // 97
    "Other Type, Reserved for future use",                 // 98
    "Other Type, no additional information",               // 99
};


} // namespace tbls


/**
 *
 * @brief Message Types grouped by common fields.
 *
 */
enum class Msg_Class {
  unknown = -1,
  m_1_2_3 = 0, ///< message types 1, 2, and 3
  m_4_11  = 1,
  m_5     = 2,
  m_6     = 3,
  m_7_13  = 4,
  m_8     = 5,
  m_9     = 6,
  m_10    = 7,
  m_12    = 8,
  m_14    = 9,
  m_15    = 10,
  m_16    = 11,
  m_17    = 12,
  m_18    = 13,
  m_19    = 14,
  m_20    = 15,
  m_21    = 16,
  m_22    = 17,
  m_23    = 18,
  m_24    = 19,
  m_25    = 20,
  m_26    = 21,
  m_27    = 22,
  m_28    = 23,
};


enum class Msg_ID : char {
  i_1  = '1',
  i_2  = '2',
  i_3  = '3',
  i_4  = '4',
  i_5  = '5',
  i_6  = '6',
  i_7  = '7',
  i_8  = '8',
  i_9  = '9',
  i_10 = ':',
  i_11 = ';',
  i_12 = '<',
  i_13 = '=',
  i_14 = '>',
  i_15 = '?',
  i_16 = '@',
  i_17 = 'A',
  i_18 = 'B',
  i_19 = 'C',
  i_20 = 'D',
  i_21 = 'E',
  i_22 = 'F',
  i_23 = 'G',
  i_24 = 'H',
  i_25 = 'I',
  i_26 = 'J',
  i_27 = 'K',
};
constexpr auto
get_msg_id(const char c) -> Msg_ID {
  return static_cast<Msg_ID>(c);
}
constexpr auto
get_msg_id(std::string_view s) -> Msg_ID {
  return static_cast<Msg_ID>(s[0]);
}
constexpr std::array all_msg_ids = {
    Msg_ID::i_1,  Msg_ID::i_2,  Msg_ID::i_3,  Msg_ID::i_4,  Msg_ID::i_5,
    Msg_ID::i_6,  Msg_ID::i_7,  Msg_ID::i_8,  Msg_ID::i_9,  Msg_ID::i_10,
    Msg_ID::i_11, Msg_ID::i_12, Msg_ID::i_13, Msg_ID::i_14, Msg_ID::i_15,
    Msg_ID::i_16, Msg_ID::i_17, Msg_ID::i_18, Msg_ID::i_19, Msg_ID::i_20,
    Msg_ID::i_21, Msg_ID::i_22, Msg_ID::i_23, Msg_ID::i_24, Msg_ID::i_25,
    Msg_ID::i_26, Msg_ID::i_27,
};

constexpr auto
get_msg_class(const Msg_ID msg_id) -> Msg_Class {
  switch (msg_id) {
    case Msg_ID::i_1:
      [[fallthrough]];
    case Msg_ID::i_2:
      [[fallthrough]];
    case Msg_ID::i_3:
      return Msg_Class::m_1_2_3;

    case Msg_ID::i_4:
      [[fallthrough]];
    case Msg_ID::i_11:
      return Msg_Class::m_4_11;

    case Msg_ID::i_5:
      return Msg_Class::m_5;

    case Msg_ID::i_6:
      return Msg_Class::m_6;

    case Msg_ID::i_7:
      [[fallthrough]];
    case Msg_ID::i_13:
      return Msg_Class::m_7_13;

    case Msg_ID::i_8:
      return Msg_Class::m_8;

    case Msg_ID::i_9:
      return Msg_Class::m_9;

    case Msg_ID::i_10:
      return Msg_Class::m_10;

    case Msg_ID::i_12:
      return Msg_Class::m_12;

    case Msg_ID::i_14:
      return Msg_Class::m_14;

    case Msg_ID::i_15:
      return Msg_Class::m_15;

    case Msg_ID::i_16:
      return Msg_Class::m_16;

    case Msg_ID::i_17:
      return Msg_Class::m_17;

    case Msg_ID::i_18:
      return Msg_Class::m_18;

    case Msg_ID::i_19:
      return Msg_Class::m_19;

    case Msg_ID::i_20:
      return Msg_Class::m_20;

    case Msg_ID::i_21:
      return Msg_Class::m_21;

    case Msg_ID::i_22:
      return Msg_Class::m_22;

    case Msg_ID::i_23:
      return Msg_Class::m_23;

    case Msg_ID::i_24:
      return Msg_Class::m_24;

    case Msg_ID::i_25:
      return Msg_Class::m_25;

    case Msg_ID::i_26:
      return Msg_Class::m_26;

    case Msg_ID::i_27:
      return Msg_Class::m_27;

    default:
      return Msg_Class::unknown;
  }
}
constexpr auto
get_msg_class(const char c) -> Msg_Class {
  return get_msg_class(get_msg_id(c));
}
constexpr auto
get_msg_class(const std::string_view s) -> Msg_Class {
  return get_msg_class(get_msg_id(s));
}
constexpr std::array all_msg_classes = {
    Msg_Class::m_1_2_3, Msg_Class::m_4_11, Msg_Class::m_5,  Msg_Class::m_6,
    Msg_Class::m_7_13,  Msg_Class::m_8,    Msg_Class::m_9,  Msg_Class::m_10,
    Msg_Class::m_12,    Msg_Class::m_14,   Msg_Class::m_15, Msg_Class::m_16,
    Msg_Class::m_17,    Msg_Class::m_18,   Msg_Class::m_19, Msg_Class::m_20,
    Msg_Class::m_21,    Msg_Class::m_22,   Msg_Class::m_23, Msg_Class::m_24,
    Msg_Class::m_25,    Msg_Class::m_26,   Msg_Class::m_27, Msg_Class::m_28,
};


enum class Field : int {
  unknown = -1,
  msg_id  = 0,
  repeat_indicator,
  mmsi,
  nav_status,
  rate_of_turn,
  speed_over_ground,
  position_accuracy,
  longitude,
  latitude,
  course_over_ground,
  true_heading,
  timestamp,
  special_maneuver,
  spare,
  spare2,
  spare3,
  spare4,
  raim,
  sync_state,
  slot_timeout,
  slot_offset,
  designated_area_code,
  functional_id,
  text,
  callsign,
  ship_name,
};


constexpr std::array all_fields = {
    Field::msg_id,
    Field::nav_status,
    Field::rate_of_turn,
    Field::speed_over_ground,
    Field::position_accuracy,
    Field::longitude,
    Field::latitude,
    Field::course_over_ground,
    Field::true_heading,
    Field::timestamp,
    Field::special_maneuver,
    Field::spare,
    Field::spare2,
    Field::spare3,
    Field::spare4,
    Field::raim,
    Field::sync_state,
    Field::slot_timeout,
    Field::slot_offset,
};


namespace meta {


// calculate meta::data::max_val for fields of n-bits
constexpr auto
bits_max_size(const std::size_t n_bits) -> std::size_t {
  constexpr decltype(n_bits) n_binary_vals = 2;
  return n_bits > 0 ? n_binary_vals * bits_max_size(n_bits - 1) : 1;
}
static_assert(bits_max_size(1) == 2);
static_assert(bits_max_size(2) == 4);
static_assert(bits_max_size(3) == 8);
static_assert(bits_max_size(4) == 16);
static_assert(bits_max_size(5) == 32);
static_assert(bits_max_size(6) == 64);
static_assert(bits_max_size(7) == 128);
static_assert(bits_max_size(8) == 256);
static_assert(bits_max_size(9) == 512);
static_assert(bits_max_size(10) == 1024);
static_assert(bits_max_size(11) == 2048);
static_assert(bits_max_size(12) == 4096);
static_assert(bits_max_size(13) == 8192);
static_assert(bits_max_size(14) == 16384);
static_assert(bits_max_size(14) == 16384);
static_assert(bits_max_size(15) == 32768);
static_assert(bits_max_size(16) == 65536);
static_assert(bits_max_size(17) == 131072);
static_assert(bits_max_size(18) == 262144);
static_assert(bits_max_size(19) == 524288);
static_assert(bits_max_size(20) == 1048576);


template <Msg_Class msg_class, Field field> struct data {
  std::size_t  first_bit{std::numeric_limits<std::size_t>::max()}; // NOLINT
  std::size_t  n_bits{0};                                          // NOLINT
  std::int64_t min_val{};                                          // NOLINT
  std::size_t  max_val{};                                          // NOLINT
  bool         has_na_val{false};                                  // NOLINT
  std::int64_t na_val{std::numeric_limits<std::int64_t>::min()};   // NOLINT
  // std::ssize_t min_val

  constexpr data() {
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> msg_id
    if constexpr (field == Field::msg_id) {
      min_val   = 1;
      max_val   = 28;
      first_bit = 0;
      n_bits    = 6;
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< msg_id


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> repeat_indicator
    if constexpr (field == Field::repeat_indicator) {
      min_val   = 0;
      max_val   = 3;
      first_bit = 6;
      n_bits    = 2;
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< repeat_indicator


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> mmsi
    if constexpr (field == Field::mmsi) {
      min_val   = 0;
      max_val   = 999'999'999;
      first_bit = 8;
      n_bits    = 30;
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< mmsi

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> nav_status
    if constexpr (field == Field::nav_status) {
      min_val = 0;
      max_val = 15;
      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        first_bit = 38;
        n_bits    = 4;
      }
      if constexpr (msg_class == Msg_Class::m_27) {
        first_bit = 40;
        n_bits    = 4;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< nav_status


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> rate_of_turn
    if constexpr (field == Field::rate_of_turn) {
      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        first_bit  = 42;
        n_bits     = 8;
        min_val    = -127;
        max_val    = 127;
        has_na_val = true;
        na_val     = -0x80;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< rate_of_turn


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> speed_over_ground
    if constexpr (field == Field::speed_over_ground) {
      min_val    = 0;
      has_na_val = true;
      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        max_val   = 1023;
        first_bit = 50;
        n_bits    = 10;
        na_val    = 1022;
      }
      if constexpr (msg_class == Msg_Class::m_18 ||
                    msg_class == Msg_Class::m_19) {
        min_val   = 0;
        max_val   = 1023;
        first_bit = 46;
        n_bits    = 10;
        na_val    = 1022;
      }
      if constexpr (msg_class == Msg_Class::m_27) {
        max_val   = 63;
        first_bit = 79;
        n_bits    = 6;
        na_val    = 63;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< speed_over_ground


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> position_accuracy
    if constexpr (field == Field::position_accuracy) {
      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        min_val   = 0;
        max_val   = 1;
        first_bit = 60;
        n_bits    = 1;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< position_accuracy


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> longitude
    if constexpr (field == Field::longitude) {
      constexpr auto divisor = []() {
        if constexpr (msg_class == Msg_Class::m_17 ||
                      msg_class == Msg_Class::m_27) {
          return 600;
        } else {
          return 600'000;
        }
      }();
      has_na_val = true;
      na_val     = 181 * divisor;
      min_val    = -180 * divisor;
      max_val    = 180 * divisor;

      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        first_bit = 61;
        n_bits    = 28;
      }
      if constexpr (msg_class == Msg_Class::m_4_11) {
        first_bit = 79;
        n_bits    = 28;
      }
      /* TODO
     if constexpr (msg_class == Msg_Class::m_6) {
       first_bit = ;
       n_bits    = ;
     }
     */
      /* TODO
      if constexpr (msg_class == Msg_Class::m_8) {
        first_bit = ;
        n_bits    = ;
      }
      */
      if constexpr (msg_class == Msg_Class::m_9) {
        first_bit = 61;
        n_bits    = 28;
      }
      if constexpr (msg_class == Msg_Class::m_17) {
        first_bit = 40;
        n_bits    = 18;
      }
      if constexpr (msg_class == Msg_Class::m_18 ||
                    msg_class == Msg_Class::m_19) {
        first_bit = 57;
        n_bits    = 28;
      }
      if constexpr (msg_class == Msg_Class::m_21) {
        first_bit = 164;
        n_bits    = 28;
      }
      if constexpr (msg_class == Msg_Class::m_27) {
        first_bit = 44;
        n_bits    = 18;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< longitude


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> latitude
    if constexpr (field == Field::latitude) {
      constexpr auto factor =
          msg_class == Msg_Class::m_17 || msg_class == Msg_Class::m_27
              ? 600
              : 600'000;
      has_na_val = true;
      na_val     = 91 * factor;
      min_val    = -90 * factor;
      max_val    = 90 * factor;
      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        first_bit = 89;
        n_bits    = 27;
      }
      if constexpr (msg_class == Msg_Class::m_4_11) {
        first_bit = 107;
        n_bits    = 27;
      }
      /* TODO
      if constexpr (msg_class == Msg_Class::m_6) {
        first_bit = ;
        n_bits    = ;
      }
      */
      /* TODO
      if constexpr (msg_class == Msg_Class::m_8) {
        first_bit = ;
        n_bits    = ;
      }
      */
      if constexpr (msg_class == Msg_Class::m_9) {
        first_bit = 89;
        n_bits    = 27;
      }
      if constexpr (msg_class == Msg_Class::m_17) {
        first_bit = 58;
        n_bits    = 17;
      }
      if constexpr (msg_class == Msg_Class::m_18 ||
                    msg_class == Msg_Class::m_19) {
        first_bit = 85;
        n_bits    = 27;
      }
      if constexpr (msg_class == Msg_Class::m_21) {
        first_bit = 192;
        n_bits    = 27;
      }
      if constexpr (msg_class == Msg_Class::m_27) {
        first_bit = 62;
        n_bits    = 17;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< latitude


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> course_over_ground
    if constexpr (field == Field::course_over_ground) {
      max_val = msg_class == Msg_Class::m_27 ? 359 : 3599;
      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        has_na_val = true;
        na_val     = 3600;
        first_bit  = 116;
        n_bits     = 12;
      }
      if constexpr (msg_class == Msg_Class::m_18 ||
                    msg_class == Msg_Class::m_19) {
        first_bit = 112;
        n_bits    = 12;
      }
      if constexpr (msg_class == Msg_Class::m_27) {
        has_na_val = true;
        na_val     = 511;
        first_bit  = 85;
        n_bits     = 9;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< course_over_ground


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> true_heading
    if constexpr (field == Field::true_heading) {
      min_val    = 0;
      max_val    = 359;
      has_na_val = true;
      na_val     = 511;
      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        first_bit = 128;
        n_bits    = 9;
      }
      if constexpr (msg_class == Msg_Class::m_18 ||
                    msg_class == Msg_Class::m_19) {
        first_bit = 124;
        n_bits    = 9;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< true_heading


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> timestamp
    if constexpr (field == Field::timestamp) {
      max_val = 59;
      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        has_na_val = true;
        na_val     = 60;
        first_bit  = 137;
        n_bits     = 6;
      }
      /* TODO
      if constexpr (msg_class == Msg_Class::m_8) {
        first_bit = ;
        n_bits    = ;
      }
      */
      if constexpr (msg_class == Msg_Class::m_9) {
        has_na_val = true;
        na_val     = 60;
        first_bit  = 128;
        n_bits     = 6;
      }
      if constexpr (msg_class == Msg_Class::m_18 ||
                    msg_class == Msg_Class::m_19) {
        first_bit = 133;
        n_bits    = 6;
      }
    }

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< timestamp


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> special_maneuver
    if constexpr (field == Field::special_maneuver) {
      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        max_val    = 2;
        has_na_val = true;
        na_val     = 0;
        first_bit  = 143;
        n_bits     = 2;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< special_maneuver


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> spare
    if constexpr (field == Field::spare) {
      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        first_bit = 145;
        n_bits    = 3;
      }
      if constexpr (msg_class == Msg_Class::m_4_11) {
        first_bit = 138;
        n_bits    = 10;
      }
      if constexpr (msg_class == Msg_Class::m_5) {
        first_bit = 423;
        n_bits    = 1;
      }
      if constexpr (msg_class == Msg_Class::m_6) {
        first_bit = 71;
        n_bits    = 1;
      }
      if constexpr (msg_class == Msg_Class::m_7_13 ||
                    msg_class == Msg_Class::m_8 ||
                    msg_class == Msg_Class::m_10 ||
                    msg_class == Msg_Class::m_14 ||
                    msg_class == Msg_Class::m_15 ||
                    msg_class == Msg_Class::m_16 ||
                    msg_class == Msg_Class::m_17 ||
                    msg_class == Msg_Class::m_20 ||
                    msg_class == Msg_Class::m_22 ||
                    msg_class == Msg_Class::m_23) {
        first_bit = 38;
        n_bits    = 2;
      }
      if constexpr (msg_class == Msg_Class::m_9) {
        // https://github.com/schwehr/libais/blob/61ed34683c01662925f4b9ed69b10885dcb3bc79/src/libais/ais9.cpp#L37
        first_bit = 135;
        n_bits    = 7;
      }
      if constexpr (msg_class == Msg_Class::m_12) {
        first_bit = 71;
        n_bits    = 1;
      }
      if constexpr (msg_class == Msg_Class::m_18 ||
                    msg_class == Msg_Class::m_19) {
        // https://github.com/schwehr/libais/blob/61ed34683c01662925f4b9ed69b10885dcb3bc79/src/libais/ais18.cpp#L56
        // https://github.com/schwehr/libais/blob/61ed34683c01662925f4b9ed69b10885dcb3bc79/src/libais/ais19.cpp#L23
        first_bit = 38;
        n_bits    = 8;
      }
      if constexpr (msg_class == Msg_Class::m_21) {
        first_bit = 271;
        n_bits    = 1;
      }
      if constexpr (msg_class == Msg_Class::m_24) {
        first_bit = 160;
        n_bits    = 8;
      }
      if constexpr (msg_class == Msg_Class::m_27) {
        first_bit = 95;
        n_bits    = 1;
      }
      max_val = bits_max_size(n_bits);
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spare


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> spare2
    if constexpr (field == Field::spare2) {
      if constexpr (msg_class == Msg_Class::m_9) {
        // https://github.com/schwehr/libais/blob/61ed34683c01662925f4b9ed69b10885dcb3bc79/src/libais/ais9.cpp#L39
        first_bit = 143;
        n_bits    = 3;
      }
      if constexpr (msg_class == Msg_Class::m_10) {
        first_bit = 70;
        n_bits    = 2;
      }
      if constexpr (msg_class == Msg_Class::m_15) {
        first_bit = 88;
        n_bits    = 2;
      }
      if constexpr (msg_class == Msg_Class::m_16 ||
                    msg_class == Msg_Class::m_17) {
        first_bit = 75;
        n_bits    = 5;
      }
      if constexpr (msg_class == Msg_Class::m_19) {
        // https://github.com/schwehr/libais/blob/61ed34683c01662925f4b9ed69b10885dcb3bc79/src/libais/ais19.cpp#L32
        first_bit = 139;
        n_bits    = 4;
      }
      if constexpr (msg_class == Msg_Class::m_22) {
        first_bit = 145;
        n_bits    = 23;
      }
      if constexpr (msg_class == Msg_Class::m_23) {
        first_bit = 122;
        n_bits    = 22;
      }
      if constexpr (msg_class == Msg_Class::m_24) {
        first_bit = 162;
        n_bits    = 6;
      }
      max_val = bits_max_size(n_bits);
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spare2


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> spare3
    if constexpr (field == Field::spare3) {
      if constexpr (msg_class == Msg_Class::m_15) {
        first_bit = 108;
        n_bits    = 2;
      }
      if constexpr (msg_class == Msg_Class::m_23) {
        first_bit = 154;
        n_bits    = 6;
      }
      max_val = bits_max_size(n_bits);
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spare3


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> spare4
    if constexpr (field == Field::spare4) {
      if constexpr (msg_class == Msg_Class::m_15) {
        first_bit = 158;
        n_bits    = 2;
      }
      max_val = bits_max_size(n_bits);
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spare4


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> raim
    if constexpr (field == Field::raim) {
      max_val = static_cast<decltype(max_val)>(true);
      if constexpr (msg_class == Msg_Class::m_1_2_3 ||
                    msg_class == Msg_Class::m_4_11) {
        first_bit = 148;
        n_bits    = 1;
      }
      if constexpr (msg_class == Msg_Class::m_9 ||
                    msg_class == Msg_Class::m_18) {
        first_bit = 147;
        n_bits    = 1;
      }
      if constexpr (msg_class == Msg_Class::m_19) {
        first_bit = 305;
        n_bits    = 1;
      }
      if constexpr (msg_class == Msg_Class::m_21) {
        first_bit = 268;
        n_bits    = 1;
      }
      if constexpr (msg_class == Msg_Class::m_27) {
        first_bit = 39;
        n_bits    = 1;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< raim


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> sync_state
    if constexpr (field == Field::sync_state) {
      if constexpr (msg_class == Msg_Class::m_1_2_3) {
        first_bit = 149;
        n_bits    = 2;
      }
      if constexpr (msg_class == Msg_Class::m_18) {
        first_bit = 149;
        n_bits    = 2;
      }
      max_val = 3;
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< sync_state


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> slot_timeout
    if constexpr (field == Field::slot_timeout) {
      if constexpr (msg_class == Msg_Class::m_1_2_3 ||
                    msg_class == Msg_Class::m_4_11 ||
                    msg_class == Msg_Class::m_18) {
        first_bit = 151;
        n_bits    = 3;
      }
      max_val = 7;
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< slot_timeout


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> slot_offset
    if constexpr (field == Field::slot_offset) {
      if constexpr (msg_class == Msg_Class::m_1_2_3 ||
                    msg_class == Msg_Class::m_4_11 ||
                    msg_class == Msg_Class::m_18) {
        first_bit = 154;
        n_bits    = 14;
      }
      /* TODO
      if constexpr (msg_class == Msg_Class::m_26) {
        first_bit = ;
        n_bits    = ;
      }
      */
      max_val = bits_max_size(n_bits);
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< slot_offset


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> designated_area_code
    if constexpr (field == Field::designated_area_code) {
      if constexpr (msg_class == Msg_Class::m_6) {
        first_bit = 72;
        n_bits    = 10;
      }
      if constexpr (msg_class == Msg_Class::m_8) {
        first_bit = 40;
        n_bits    = 10;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< designated_area_code


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> functional_id
    if constexpr (field == Field::functional_id) {
      if constexpr (msg_class == Msg_Class::m_6) {
        first_bit = 82;
        n_bits    = 6;
      }
      if constexpr (msg_class == Msg_Class::m_8) {
        first_bit = 50;
        n_bits    = 6;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< functional_id


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> text
    if constexpr (field == Field::text) {
      if constexpr (msg_class == Msg_Class::m_12) {
        first_bit = 72;
        n_bits    = 936;
      }
      if constexpr (msg_class == Msg_Class::m_14) {
        first_bit = 40;
        n_bits    = 966;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< text


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> callsign
    if constexpr (field == Field::callsign) {
      if constexpr (msg_class == Msg_Class::m_5) {
        first_bit = 70;
        n_bits    = 42;
      }
      if constexpr (msg_class == Msg_Class::m_24) {
        first_bit = 90;
        n_bits    = 42;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< callsign


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ship_name
    if constexpr (field == Field::ship_name) {
      if constexpr (msg_class == Msg_Class::m_5) {
        first_bit = 112;
        n_bits    = 120;
      }
      if constexpr (msg_class == Msg_Class::m_19) {
        first_bit = 143;
        n_bits    = 120;
      }
      if constexpr (msg_class == Msg_Class::m_24) {
        first_bit = 40;
        n_bits    = 120;
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ship_name
  }
};


} // namespace meta


template <Msg_Class msg_class, Field field>
constexpr auto
has_field() noexcept -> bool {
  return meta::data<msg_class, field>().n_bits != 0;
}


template <Msg_Class msg_class>
constexpr auto
has_field(const Field f) noexcept -> bool {
  switch (f) {
    case Field::course_over_ground:
      return has_field<msg_class, Field::course_over_ground>();
    case Field::latitude:
      return has_field<msg_class, Field::latitude>();
    case Field::longitude:
      return has_field<msg_class, Field::longitude>();
    default:
      return false;
  }
}


template <std::size_t First, typename F, std::size_t... Is>
constexpr void
constexpr_for_impl(F&&                        f,
                   std::index_sequence<Is...> _) { // NOLINT
  (f(std::integral_constant<std::size_t, First + Is>{}), ...);
}

template <typename T, T First, T Last, typename F>
constexpr void
constexpr_for(F&& f) {
  constexpr_for_impl<First>(std::forward<F>(f),
                            std::make_index_sequence<Last - First>{});
}


template <Msg_Class msg_class>
constexpr auto
count_fields() -> std::size_t {
  std::size_t out{0};
  constexpr_for<std::size_t, 0, std::size(all_fields)>([&](auto i) {
    if constexpr (has_field<msg_class, std::get<i>(all_fields)>()) {
      ++out;
    }
  });
  return out;
}


template <Msg_Class msg_class>
constexpr auto
get_fields() {
  std::array<Field, count_fields<msg_class>()> out{};
  auto*                                        it = std::begin(out);
  constexpr_for<std::size_t, 0, std::size(all_fields)>([&](auto i) {
    if constexpr (has_field<msg_class, std::get<i>(all_fields)>()) {
      *it++ = std::get<i>(all_fields); // NOLINT
    }
  });

  return out;
}


} // namespace aiscpp


#endif
