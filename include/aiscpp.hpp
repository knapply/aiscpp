/**
 * @file aiscpp.hpp
 * @author Brendan Knapp (brendan.g.knapp@gmail.com)
 * @brief
 * @version 0.1.1
 * @date 2021-01-17
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef AISCPP_HPP_
#define AISCPP_HPP_


#ifndef __cplusplus
#  error This is not a C++ compiler.
#elif __cplusplus < 201703L
#  error This is not a C++>=17 compiler.
#endif


#ifdef _MSC_VER
#  define AISCPP_VISUAL_STUDIO 1
#  ifdef __clang__
#    define AISCPP_CLANG_VISUAL_STUDIO 1
#  else
#    define AISCPP_REGULAR_VISUAL_STUDIO 1
#  endif // __clang__
#endif   // _MSC_VER


#if defined(AISCPP_REGULAR_VISUAL_STUDIO) && AISCPP_REGULAR_VISUAL_STUDIO
#  define aiscpp_really_inline __forceinline
#else
#  define aiscpp_really_inline inline __attribute__((always_inline, unused))
#endif // defined(AISCPP_REGULAR_VISUAL_STUDIO) && AISCPP_REGULAR_VISUAL_STUDIO


#ifndef AISCPP_ENABLE_STATIC_TESTS
#  define AISCPP_ENABLE_STATIC_TESTS 1
#endif
#ifndef AISCPP_ENABLE_STATIC_TESTS_FLOATING_POINT
#  define AISCPP_ENABLE_STATIC_TESTS_FLOATING_POINT AISCPP_ENABLE_STATIC_TESTS
#endif


#include <type_traits>
#ifndef __cpp_lib_remove_cvref
namespace std {
template <class T> struct remove_cvref {
  typedef remove_cv_t<std::remove_reference_t<T>> type;
};
} // namespace std
#endif // __cpp_lib_remove_cvref


#include <array>
#include <charconv>
#include <limits>
#include <string>
#include <string_view>


#include "aiscpp_metadata.hpp"


namespace aiscpp {


/*
 * @brief Error Codes
 */
enum class status_code {
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> notes/warnings
  SPEED_OVER_GROUND_EXCEEDS_102_POINT_2_KNOTS          = -4,
  POSITIONING_SYSTEM_IN_MANUAL_INPUT_MODE              = -3,
  ELECTRONIC_POSITION_FIXING_SYSTEMS_IN_ESTIMATED_MODE = -2,
  POSITIONING_SYSTEM_INOPERATIVE                       = -1,
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< notes/warnings
  SUCCESS = 0,
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> errors/bad data
  NOT_AVAILABLE,
  NOT_DEFINED,
  MSG_TYPE_HAS_NO_SUCH_FIELD,
  UNKNOWN,
  UNINITIALIZED,
  CHECKSUM_TARGET_INVALID,
  CHECKSUM_BAD_MATCH,
  MSG_LENGTH_TOO_SHORT,
  MSG_LENGTH_TOO_LONG,
  INVALID_FIRST_CHAR,
  FILL_BITS_INVALID,
  LESS_THAN_6_COMMAS,
  FIELD_EMPTY,
  MEMBER_NOT_PRESENT,
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< errors
};

/* `static_assert()` messages must bein inline text (or a macro thereof) */
#define AISCPP_BAD_FIELD "This `Msg_Class` doesn't have the requested data."


/**
 * The result of user-facing operations.
 *
 */
template <typename T> struct aiscpp_result {
  T           value_{};
  status_code status_{};

  constexpr aiscpp_result() noexcept
      : value_(T{}), status_(status_code::UNINITIALIZED) {}

  constexpr aiscpp_result(status_code status) noexcept
      : value_(T{}), status_(status) {}

  constexpr aiscpp_result(T&& value) noexcept
      : value_(value), status_(status_code::SUCCESS) {}

  constexpr aiscpp_result(T&& value, status_code status_code) noexcept
      : value_(value), status_(status_code) {}

  template <typename U>
  constexpr aiscpp_result(U&& value, status_code status) noexcept
      : value_(value), status_(status) {}

  template <typename U>
  constexpr aiscpp_result(U& value) noexcept
      : value_(value), status_(status_code::SUCCESS) {}

  [[nodiscard]] constexpr auto
  status() const noexcept -> status_code {
    return this->status_;
  }

  [[nodiscard]] constexpr auto
  value() const noexcept -> T {
    return this->value_;
  }

  constexpr
  operator T&&() && noexcept {
    return std::forward<T>((*this).value_);
  }

  // support exceptions?????????????????????????????????
  // constexpr auto
  // take_value() && noexcept -> T&& {
  //   return std::forward<T>((*this).value_);
  // }
  // template <typename U>
  // constexpr auto
  // operator==(U rhs) const noexcept -> bool {
  //   return this->value_ == rhs;
  // }
  // constexpr auto
  // tie(T& value, status_code& error) && noexcept -> void {
  //   if (error = this->error; error == status_code::SUCCESS) {
  //     value = this->take_value();
  //   }
  // }
  // constexpr auto
  // get(T& value) && noexcept -> status_code {
  //   status_code status{};
  //   std::forward<T>(*this).tie(value, status);
  //   return status;
  // }
  // constexpr std::pair() && noexcept {
  //   return std::make_pair(std::forward<T>(value_),
  //                         std::forward<status_code>(status_));
  // }
};


} // namespace aiscpp


// #include "common.hpp"
// #include "aiscpp_result.hpp"


#define AISCPP_VERSION_MAJOR 0
#define AISCPP_VERSION_MINOR 1
#define AISCPP_VERSION_REVISION 1
namespace aiscpp {
namespace version_info {
struct aiscpp_version_info {
  static constexpr int major    = AISCPP_VERSION_MAJOR;
  static constexpr int minor    = AISCPP_VERSION_MINOR;
  static constexpr int revision = AISCPP_VERSION_REVISION;
};
} // namespace version_info
static constexpr auto aiscpp_version = version_info::aiscpp_version_info{};
static_assert(aiscpp_version.major == 0);
static_assert(aiscpp_version.minor == 1);
static_assert(aiscpp_version.revision == 1);
} // namespace aiscpp


namespace aiscpp {

namespace bitops {

using bits_T = std::uint32_t;


aiscpp_really_inline constexpr auto
to_ascii6(char c) noexcept -> bits_T {
  return c - 48 - ((c > 88) * 8);
  // return c <= 88 ? static_cast<bits_T>(c - 48) : static_cast<bits_T>(c - 56);
}

/*
 *                                      [1 byte (`char`)]
 * `char` bit indices              =>   [7|6|5|4|3|2|1|0]
 * 7-bit ASCII (standard) indices  =>   [ |6|5|4|3|2|1|0]
 * 6-bit ASCII (NMEA 0183) indices =>   [ | |5|4|3|2|1|0]
 *
 *  example: ABC
 * `char`            [       A       ][         B       ][          C          ]
 * `char` index      [       0       ][         1       ][          2          ]
 * per `char` index  [ | |5|4|3|2|1|0][ | | 5| 4|3|2|1|0][ | | 5| 4| 3| 2| 1| 0]
 * all bit indices   [ | |5|4|3|2|1|0][ | |11|10|9|8|7|6][ | |17|16|15|14|13|12]
 */
template <std::size_t Start, std::size_t Length, typename out_T = std::uint32_t>
aiscpp_really_inline constexpr auto
decode_number(std::string_view s) noexcept -> out_T {
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> problem children
  // constexpr auto Start  = 40;  // fixed
  // constexpr auto Length = 4;   // fixed
  // constexpr auto Start  = 139; // fixed
  // constexpr auto Length = 4;   // fixed
  // constexpr auto Start  = 158; // fixed
  // constexpr auto Length = 2;   // fixed
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< problem children

  static_assert(std::is_arithmetic_v<out_T>);
  static_assert(Start >= 0 && Length > 0);

  constexpr std::size_t N_BITS_PER_CHAR  = 6;
  constexpr auto        LAST_BIT_IN_CHAR = N_BITS_PER_CHAR - 1;
  // clang-format off
  constexpr auto I_FIRST_CHAR            = Start / N_BITS_PER_CHAR;
  constexpr auto I_BIT_FIRST_CHAR        = Start % N_BITS_PER_CHAR;
  constexpr auto N_BITS_FIRST_CHAR       = ((Length < N_BITS_PER_CHAR) && ((I_BIT_FIRST_CHAR + Length) <= LAST_BIT_IN_CHAR)) ? Length : (N_BITS_PER_CHAR - I_BIT_FIRST_CHAR);
  constexpr auto N_BITS_AFTER_FIRST_CHAR = Length - N_BITS_FIRST_CHAR;
  constexpr auto N_BITS_LAST_CHAR        = N_BITS_AFTER_FIRST_CHAR % 6;
  constexpr auto N_MID_CHARS             = (N_BITS_AFTER_FIRST_CHAR - N_BITS_LAST_CHAR) / 6;
  constexpr auto I_LAST_CHAR             = I_FIRST_CHAR + N_MID_CHARS + 1;
  // clang-format on
  constexpr bits_T B0{0};
  constexpr bits_T B1{1};
  constexpr bits_T B2{2};
  constexpr bits_T B3{3};
  constexpr bits_T B4{4};
  constexpr bits_T B5{5};

  constexpr auto MINUEND = LAST_BIT_IN_CHAR - I_BIT_FIRST_CHAR;

  if constexpr (std::is_unsigned_v<out_T>) {
    bits_T res{0};

    for (bits_T i_bit{0}, first_char{to_ascii6(s[I_FIRST_CHAR])};
         i_bit < N_BITS_FIRST_CHAR;
         ++i_bit) {
      res = (res << B1) | (B1 & first_char >> (MINUEND - i_bit));
    }
    if constexpr (N_MID_CHARS != 0) {
      for (std::size_t i_char{I_FIRST_CHAR + 1}; i_char < I_LAST_CHAR;
           ++i_char) {
        const auto ascii6{to_ascii6(s[i_char])};
        res = (res << B1) | (B1 & ascii6 >> B5);
        res = (res << B1) | (B1 & ascii6 >> B4);
        res = (res << B1) | (B1 & ascii6 >> B3);
        res = (res << B1) | (B1 & ascii6 >> B2);
        res = (res << B1) | (B1 & ascii6 >> B1);
        res = (res << B1) | (B1 & ascii6 >> B0);
      }
    }
    if constexpr (N_BITS_LAST_CHAR != 0) {
      const auto last_char = to_ascii6(s[I_LAST_CHAR]);
      for (bits_T i_bit{0}; i_bit < N_BITS_LAST_CHAR; ++i_bit) {
        res = (res << B1) | (B1 & last_char >> (LAST_BIT_IN_CHAR - i_bit));
      }
    }

    return static_cast<out_T>(res);

  } else { // !std::is_unsigned_v<out_T>
    const auto first_char = to_ascii6(s[I_FIRST_CHAR]);
    if (/* is_pos */ !(first_char & (B1 << MINUEND))) { //  use unsigned branch
      return static_cast<out_T>(decode_number<Start, Length, bits_T>(s));
    }

    bits_T res{0};

    for (bits_T i_bit{0}; i_bit < N_BITS_FIRST_CHAR; ++i_bit) {
      res = (res << B1) | (B0 == (B1 & first_char >> (MINUEND - i_bit)));
    }
    if constexpr (N_MID_CHARS != 0) {
      for (auto i_char{I_FIRST_CHAR + 1}; i_char < I_LAST_CHAR; ++i_char) {
        const auto ascii6 = to_ascii6(s[i_char]);
        res               = (res << B1) | (B0 == (B1 & ascii6 >> B5));
        res               = (res << B1) | (B0 == (B1 & ascii6 >> B4));
        res               = (res << B1) | (B0 == (B1 & ascii6 >> B3));
        res               = (res << B1) | (B0 == (B1 & ascii6 >> B2));
        res               = (res << B1) | (B0 == (B1 & ascii6 >> B1));
        res               = (res << B1) | (B0 == (B1 & ascii6 >> B0));
      }
    }
    if constexpr (N_BITS_LAST_CHAR != 0) {
      for (bits_T i_bit{0}, last_char{to_ascii6(s[I_LAST_CHAR])};
           i_bit < N_BITS_LAST_CHAR;
           ++i_bit) {
        res = (res << B1) |
              (B0 == (B1 & last_char >> (LAST_BIT_IN_CHAR - i_bit)));
      }
    }

    return -(static_cast<out_T>(res) + 1);
  }
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
constexpr auto m_1_1 = "177KQJ5000G?tO`K>RA1wUbN0TKH";
static_assert(decode_number<0, 6>(m_1_1) == 1);                // message ID
static_assert(decode_number<6, 2>(m_1_1) == 0);                // repeat
static_assert(decode_number<8, 30>(m_1_1) == 477553000);       // MMSI
static_assert(decode_number<38, 4>(m_1_1) == 5);               // nav_status
static_assert(decode_number<61, 28, int>(m_1_1) == -73407500); // longitude
static_assert(decode_number<89, 27, int>(m_1_1) == 28549700);  // latitude
static_assert(decode_number<149, 2>("177KQJ5000G?tO`K>RA1wUbN0TKH") == 1);
} // namespace static_tests
#endif


template <std::size_t Start, std::size_t Length, std::size_t N_CHARS = 161>
constexpr auto
decode_chars(std::string_view s) noexcept {
  static_assert(Length % 6 == 0);

  constexpr char ASCII6[] = // NOLINT
      R"(@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^- !"#$%&`()*+,-./0123456789:;<=>?)";
  constexpr auto build_init_chars = []() constexpr {
    static_assert(N_CHARS >= Length / 6);
    std::array<char, N_CHARS> init_chars{};
    for (auto& c : init_chars) {
      c = '@';
    }
    return init_chars;
  };
  constexpr auto REMAINDER = Length % 6;
  constexpr auto START     = Start % 6;
  constexpr auto I_CHAR    = Start / 6;
  // clang-format off
  constexpr auto N_CHAR = I_CHAR + (Length / 6) + (REMAINDER != 0 ? 1 : 0) + (START != 0 ? 1 : 0);
  // clang-format on
  constexpr auto   BS_SIZE = N_CHAR * 6;
  constexpr bits_T B0{0};
  constexpr bits_T B1{1};
  constexpr bits_T B2{2};
  constexpr bits_T B3{3};
  constexpr bits_T B4{4};
  constexpr bits_T B5{5};

  bool bs[BS_SIZE]{}; // NOLINT

  for (std::size_t i_char = I_CHAR, i_bit = 0;
       i_char < N_CHAR && i_char < std::size(s);
       ++i_char) {
    const auto ascii6 = to_ascii6(s[i_char]);
    bs[i_bit++]       = (ascii6 >> B5) & B1;
    bs[i_bit++]       = (ascii6 >> B4) & B1;
    bs[i_bit++]       = (ascii6 >> B3) & B1;
    bs[i_bit++]       = (ascii6 >> B2) & B1;
    bs[i_bit++]       = (ascii6 >> B1) & B1;
    bs[i_bit++]       = (ascii6 >> B0) & B1;
  }

  auto chars    = build_init_chars();
  auto it_chars = std::begin(chars);
  for (std::size_t i_bit = START; i_bit < START + Length;) {
    bits_T res{0};
    res = (res << B1) | static_cast<bits_T>(bs[i_bit++]);
    res = (res << B1) | static_cast<bits_T>(bs[i_bit++]);
    res = (res << B1) | static_cast<bits_T>(bs[i_bit++]);
    res = (res << B1) | static_cast<bits_T>(bs[i_bit++]);
    res = (res << B1) | static_cast<bits_T>(bs[i_bit++]);
    res = (res << B1) | static_cast<bits_T>(bs[i_bit++]);
    if (const auto c = ASCII6[static_cast<std::size_t>(res)]; c != '@') {
      *it_chars++ = c;
    } else {
      return chars;
    }
  }

  while (*--it_chars == ' ') {
    *it_chars = '@'; // NOLINT
  }
  return chars;
}


template <std::size_t N>
constexpr auto
trim_chars(std::array<char, N>&& chars) -> std::string_view {
  return std::string_view(
      std::cbegin(chars),
      static_cast<std::string_view::size_type>(
          std::char_traits<char>::find(std::cbegin(chars), N, '@') -
          std::cbegin(chars)) //
  );
}


template <std::size_t N>
constexpr auto
trim_chars(aiscpp_result<std::array<char, N>>&& result) -> std::string_view {
  return trim_chars(std::forward<decltype(result.value_)>(result.value_));
}


template <std::size_t Start, std::size_t Length>
auto
decode_str(std::string_view s) noexcept -> std::string {
  return std::string(trim_chars(decode_chars<Start, Length>(s)));
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests { // clang-format off
constexpr auto m_5__1 = "53eaFL02?;fwTPm7V219E@R1@PE8E<622222221@9hG1A7?@NCPSlm3kc5DhH8888888880";
static_assert(std::string_view(decode_chars<70, 42>(m_5__1).begin(),  42 / 6) == "9HMQ9@@");
static_assert(std::string_view(decode_chars<302, 120>(m_5__1).begin(),  120 / 6) == "BOSTON,USA@@@@@@@@@@");

static_assert(trim_chars(decode_chars<70, 42>(m_5__1)) == "9HMQ9");
static_assert(trim_chars(decode_chars<302, 120>(m_5__1)) == "BOSTON,USA");

constexpr auto m_12__1 = "<5MwpVn0AAup=C7P6B?=Pknnqqqoho0<5MwpVr0AAup=C7P6B?=Pknnqqqoho0<5MwpVr0AAup=C7P6B?=Pknnqqqoho0";
static_assert(std::string_view(decode_chars<72, 936>(m_12__1).begin(), 936 / 6) == "MSG FROM 366999707@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
static_assert(trim_chars(decode_chars<72, 936>(m_12__1)) == "MSG FROM 366999707");
// clang-format on
} // namespace static_tests
#endif
} // namespace bitops



#if AISCPP_ENABLE_STATIC_TESTS
namespace static_data {
constexpr auto
extract_payload(const std::string_view s) -> std::string_view {
  const auto* left = std::cbegin(s);
  // std::size_t comma_count{0};
  for (const auto* right =
           std::char_traits<char>::find(left, std::size(s), ',');
       right - std::cbegin(s) < std::size(s);
       right = std::char_traits<char>::find(left, std::cend(s) - left, ',')) {
    // if (++comma_count == 6) {
    if (*(right + 2) == '*') {
      return std::string_view(left, right - left);
    }
    left = right + 1; // NOLINT
  }
  return "";
}
// clang-format off
constexpr auto ai_1_a = "!AIVDM,1,1,,B,177KQJ5000G?tO`K>RA1wUbN0TKH,0*5C";
constexpr auto pl_1_a = extract_payload(ai_1_a);
static_assert(pl_1_a == "177KQJ5000G?tO`K>RA1wUbN0TKH");

constexpr auto ai_1_b = "!AIVDM,1,1,,A,15N1u<PP1FJuvSRHOE6QIwwh0HQ6,0*30,d-080,S2118,t050056.00,T56.4837741,r003669946,1325394060,1325394001";
constexpr auto pl_1_b = extract_payload(ai_1_b);
static_assert(pl_1_b == "15N1u<PP1FJuvSRHOE6QIwwh0HQ6");

constexpr auto ai_2_a = "!AIVDM,1,1,,B,2T8Mur>pJNw77ks5q?7mdfhoOgMo,0*3A";
constexpr auto pl_2_a = extract_payload(ai_2_a);
static_assert(pl_2_a == "2T8Mur>pJNw77ks5q?7mdfhoOgMo");

constexpr auto ai_3_a = "!AIVDM,1,1,,B,35MC>W@01EIAn5VA4l`N2;>0015@,0*01";
constexpr auto pl_3_a = extract_payload(ai_3_a);
static_assert(pl_3_a == "35MC>W@01EIAn5VA4l`N2;>0015@");

constexpr auto ai_4_a = "!AIVDM,1,1,,A,403Ovl@000Htt<tSF0l4Q@100`Pq,0*28,d-109,S2105,t050056.00,T56.13718694,r003669946,1325394060,1325394001";
constexpr auto pl_4_a = extract_payload(ai_4_a);
static_assert(pl_4_a == "403Ovl@000Htt<tSF0l4Q@100`Pq");

constexpr auto ai_5_a = "!AIVDM,1,1,1,B,53eaFL02?;fwTPm7V219E@R1@PE8E<622222221@9hG1A7?@NCPSlm3kc5DhH8888888880,2*7F";
constexpr auto pl_5_a = extract_payload(ai_5_a);
static_assert(pl_5_a == "53eaFL02?;fwTPm7V219E@R1@PE8E<622222221@9hG1A7?@NCPSlm3kc5DhH8888888880");

constexpr auto ai_6_a = "!AIVDM,1,1,,A,603Owlh0>5Q@04000000,0*01";
constexpr auto pl_6_a = extract_payload(ai_6_a);
static_assert(pl_6_a == "603Owlh0>5Q@04000000");

constexpr auto ai_7_a = "!AIVDM,1,1,,A,71AqOpwIFFI:vJCjWejS9I:QGsFw,0*62";
constexpr auto pl_7_a = extract_payload(ai_7_a);
static_assert(pl_7_a == "71AqOpwIFFI:vJCjWejS9I:QGsFw");

constexpr auto ai_8_a = "!AIVDM,1,1,,B,803Ovrh0EP:024`@02PN04da=3V<>N0000,4*39";
constexpr auto pl_8_a = extract_payload(ai_8_a);
static_assert(pl_8_a == "803Ovrh0EP:024`@02PN04da=3V<>N0000");

constexpr auto ai_9_a = "!AIVDM,1,1,,B,9muc;QvN2`TG6BE76wm;U7u>K>?b,0*64";
constexpr auto pl_9_a = extract_payload(ai_9_a);
static_assert(pl_9_a == "9muc;QvN2`TG6BE76wm;U7u>K>?b");

constexpr auto ai_10_a = "!AIVDM,1,1,,A,:5C=uP0j4I3P,0*36";
constexpr auto pl_10_a = extract_payload(ai_10_a);
static_assert(pl_10_a == ":5C=uP0j4I3P");

constexpr auto ai_11_a = "!AIVDM,1,1,,A,;8IVO`1ua>0:pqGjwL?i6P100000,0*22";
constexpr auto pl_11_a = extract_payload(ai_11_a);
static_assert(pl_11_a == ";8IVO`1ua>0:pqGjwL?i6P100000");

// 12
constexpr auto ai_12_a = "!AIVDM,1,1,,B,<rOwUArW>mATsl8mEu6cvWeww1gsMlTPAh,4*62";
constexpr auto pl_12_a = extract_payload(ai_12_a);
static_assert(pl_12_a == "<rOwUArW>mATsl8mEu6cvWeww1gsMlTPAh");

constexpr auto ai_12_b = "!AIVDM,2,1,1,A,<02PeAPpIkF06B?=PB?31P3?>DB?<rP@<51C5P3?>D13DPB?31P3?>DB,0*13";
constexpr auto pl_12_b = extract_payload(ai_12_b);
static_assert(pl_12_b == "<02PeAPpIkF06B?=PB?31P3?>DB?<rP@<51C5P3?>D13DPB?31P3?>DB");

// 13
constexpr auto ai_13_a = "!AIVDM,1,1,,A,=8157oQGOv9f,0*0F";
constexpr auto pl_13_a = extract_payload(ai_13_a);
static_assert(pl_13_a == "=8157oQGOv9f");

constexpr auto ai_14_a = "!AIVDM,1,1,,B,>Co2MsFw3gwqfk=6uisakraA`@dS8l`,2*48";
constexpr auto pl_14_a = extract_payload(ai_14_a);
static_assert(pl_14_a == ">Co2MsFw3gwqfk=6uisakraA`@dS8l`");

constexpr auto ai_15_a = "!AIVDM,1,1,,A,?5N29b18w<3PD00,2*6C";
constexpr auto pl_15_a = extract_payload(ai_15_a);
static_assert(pl_15_a == "?5N29b18w<3PD00");

constexpr auto ai_16_a = "!AIVDM,1,1,,A,@pCdoqggJc1KHfk`lIowv;WB,0*3C";
constexpr auto pl_16_a = extract_payload(ai_16_a);
static_assert(pl_16_a == "@pCdoqggJc1KHfk`lIowv;WB");

constexpr auto ai_17_a = "!AIVDM,1,1,,A,Asgw4Gg:94e7evilw4kKvvwHbA8i@V:QEEP,2*62,d-127,S0388,t032710.00,T10.35675633,r09NALP1,1272425231";
constexpr auto pl_17_a = extract_payload(ai_17_a);
static_assert(pl_17_a == "Asgw4Gg:94e7evilw4kKvvwHbA8i@V:QEEP");

constexpr auto ai_18_a = "!AIVDM,1,1,,B,BE2K5MP005kN8WVSrcP03wb5oP00,0*54,d-085,S0782,t000020.00,T20.85679339,r13CSPO1,1272412821";
constexpr auto pl_18_a = extract_payload(ai_18_a);
static_assert(pl_18_a == "BE2K5MP005kN8WVSrcP03wb5oP00");

constexpr auto ai_19_a = "!AIVDM,1,1,6,A,C5MtL4eP0FK?P@4I96hG`urH@2fF0000000000000000?P000020,0*4D,d-081,S1988,t002853.00,T53.01528638,r08RROB1,1272414533";
constexpr auto pl_19_a = extract_payload(ai_19_a);
static_assert(pl_19_a == "C5MtL4eP0FK?P@4I96hG`urH@2fF0000000000000000?P000020");

constexpr auto ai_20_a = "!AIVDM,1,1,,A,D02u=ThfmNfpnaN9H0,4*43";
constexpr auto pl_20_a = extract_payload(ai_20_a);
static_assert(pl_20_a == "D02u=ThfmNfpnaN9H0");

constexpr auto ai_21_a = "!AIVDM,1,1,,A,E4eGk5?2Qh;Pb2a@8bPV4b<h1:WsVHh1>3t4p00000dP0V@,2*2E,b003669705,1272413636";
constexpr auto pl_21_a = extract_payload(ai_21_a);
static_assert(pl_21_a == "E4eGk5?2Qh;Pb2a@8bPV4b<h1:WsVHh1>3t4p00000dP0V@");

constexpr auto ai_22_a = "!AIVDM,1,1,,A,F030p2j2N2P6S3iW8=4P3;r20000,0*24";
constexpr auto pl_22_a = extract_payload(ai_22_a);
static_assert(pl_22_a == "F030p2j2N2P6S3iW8=4P3;r20000");

constexpr auto ai_23_a = "!AIVDM,1,1,,A,G2IT9vuB6dus:bgcCtm4SUMrVgL,2*16";
constexpr auto pl_23_a = extract_payload(ai_23_a);
static_assert(pl_23_a == "G2IT9vuB6dus:bgcCtm4SUMrVgL");

constexpr auto ai_24_a = "!AIVDM,1,1,,B,H5NHBTTU653hhhiG45oqkp1@D340,0*39";
constexpr auto pl_24_a = extract_payload(ai_24_a);
static_assert(pl_24_a == "H5NHBTTU653hhhiG45oqkp1@D340");

constexpr auto ai_25_a = "!AIVDM,1,1,,B,I5Mwp<BQ=?rKuen5Q;Na8DncdWWN,0*52";
constexpr auto pl_25_a = extract_payload(ai_25_a);
static_assert(pl_25_a == "I5Mwp<BQ=?rKuen5Q;Na8DncdWWN");

constexpr auto ai_26_a = "!AIVDM,1,1,,B,JFDwLv9U;AG0gOLROo<U,0*3E";
constexpr auto pl_26_a = extract_payload(ai_26_a);
static_assert(pl_26_a == "JFDwLv9U;AG0gOLROo<U");

constexpr auto ai_27_a = "!AIVDM,1,1,,A,KvQ:1o`7EBrBQ`?w,0*37,d-127,S0258,t111606.00,T06.8887855,r003669946,1325416567,1325416509";
constexpr auto pl_27_a = extract_payload(ai_27_a);
static_assert(pl_27_a ==  "KvQ:1o`7EBrBQ`?w");

constexpr auto ai_27_b = "!AIVDM,1,1,,A,KvWMIfCNvoNqRAsh,0*0D";
constexpr auto pl_27_b = extract_payload(ai_27_b);
static_assert(pl_27_b ==  "KvWMIfCNvoNqRAsh");

constexpr auto ai_27_c = "!AIVDM,1,1,,B,Kq2Qm7R2JNtjVq52,0*7C";
constexpr auto pl_27_c = extract_payload(ai_27_c);
static_assert(pl_27_c ==  "Kq2Qm7R2JNtjVq52");
// clang-format on
} // namespace static_data
#endif


namespace decode_internal {

template <Msg_Class msg_class, Field field, typename val_T>
constexpr auto
decode_num(std::string_view s) noexcept -> aiscpp_result<val_T> {
  if constexpr (msg_class != Msg_Class::unknown ||
                field == Field::repeat_indicator || field == Field::msg_id ||
                field == Field::mmsi) {
    static_assert(has_field<msg_class, field>(), AISCPP_BAD_FIELD);

    constexpr auto md = meta::data<msg_class, field>{};
    const auto out = bitops::decode_number<md.first_bit, md.n_bits, val_T>(s);
    if constexpr (md.has_na_val) {
      if (out == md.na_val) {
        return {out, status_code::NOT_AVAILABLE};
      }
    }
    if constexpr (std::is_unsigned_v<val_T>) {
      if (out > md.max_val) {
        return {out, status_code::NOT_DEFINED};
      }
    } else {
      if (out < md.min_val || out > md.max_val) {
        return {out, status_code::NOT_DEFINED};
      }
    }
    return out;

  } else {
    switch (get_msg_class(s)) {
      case Msg_Class::m_1_2_3: {
        if constexpr (has_field<Msg_Class::m_1_2_3, field>()) {
          return decode_num<Msg_Class::m_1_2_3, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_4_11: {
        if constexpr (has_field<Msg_Class::m_4_11, field>()) {
          return decode_num<Msg_Class::m_4_11, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_5: {
        if constexpr (has_field<Msg_Class::m_5, field>()) {
          return decode_num<Msg_Class::m_5, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_6: {
        if constexpr (has_field<Msg_Class::m_6, field>()) {
          return decode_num<Msg_Class::m_6, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_7_13: {
        if constexpr (has_field<Msg_Class::m_7_13, field>()) {
          return decode_num<Msg_Class::m_7_13, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_8: {
        if constexpr (has_field<Msg_Class::m_8, field>()) {
          return decode_num<Msg_Class::m_8, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_9: {
        if constexpr (has_field<Msg_Class::m_9, field>()) {
          return decode_num<Msg_Class::m_9, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_10: {
        if constexpr (has_field<Msg_Class::m_10, field>()) {
          return decode_num<Msg_Class::m_10, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_12: {
        if constexpr (has_field<Msg_Class::m_12, field>()) {
          return decode_num<Msg_Class::m_12, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_14: {
        if constexpr (has_field<Msg_Class::m_14, field>()) {
          return decode_num<Msg_Class::m_14, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_15: {
        if constexpr (has_field<Msg_Class::m_15, field>()) {
          return decode_num<Msg_Class::m_15, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_16: {
        if constexpr (has_field<Msg_Class::m_16, field>()) {
          return decode_num<Msg_Class::m_16, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_17: {
        if constexpr (has_field<Msg_Class::m_17, field>()) {
          return decode_num<Msg_Class::m_17, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_18: {
        if constexpr (has_field<Msg_Class::m_18, field>()) {
          return decode_num<Msg_Class::m_18, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_19: {
        if constexpr (has_field<Msg_Class::m_19, field>()) {
          return decode_num<Msg_Class::m_19, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_20: {
        if constexpr (has_field<Msg_Class::m_20, field>()) {
          return decode_num<Msg_Class::m_20, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_21: {
        if constexpr (has_field<Msg_Class::m_21, field>()) {
          return decode_num<Msg_Class::m_21, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_22: {
        if constexpr (has_field<Msg_Class::m_22, field>()) {
          return decode_num<Msg_Class::m_22, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_23: {
        if constexpr (has_field<Msg_Class::m_23, field>()) {
          return decode_num<Msg_Class::m_23, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_24: {
        if constexpr (has_field<Msg_Class::m_24, field>()) {
          return decode_num<Msg_Class::m_24, field, val_T>(s);
        }
      } break;
      case Msg_Class::m_25: {
        if constexpr (has_field<Msg_Class::m_25, field>()) {
          return decode_num<Msg_Class::m_25, field, val_T>(s);
        }
      } break;

      case Msg_Class::m_26: {
        if constexpr (has_field<Msg_Class::m_26, field>()) {
          return decode_num<Msg_Class::m_26, field, val_T>(s);
        }
      } break;

      case Msg_Class::m_27: {
        if constexpr (has_field<Msg_Class::m_27, field>()) {
          return decode_num<Msg_Class::m_27, field, val_T>(s);
        }
      } break;

      default:
        return status_code::UNKNOWN;
    }
  }
  return status_code::MSG_TYPE_HAS_NO_SUCH_FIELD;
}

// template <Msg_Class msg_class, Field field, typename val_T = std::int32_t>
// constexpr auto
// decode_signed(std::string_view s) noexcept -> aiscpp_result<val_T> {
//   static_assert(std::is_signed_v<val_T>);
//   return decode_num<msg_class, field, val_T>(s);
// }

// template <Msg_Class msg_class, Field field, typename val_T = std::uint32_t>
// constexpr auto
// decode_unsigned(std::string_view s) noexcept -> aiscpp_result<val_T> {
//   static_assert(std::is_unsigned_v<val_T>);
//   return decode_num<msg_class, field, val_T>(s);
// }


} // namespace decode_internal


/////////////////////////////////////////////////////////////////////////////////
template <Field> struct Result_Value {};
template <Field field> using value_T = typename Result_Value<field>::type;

/*
 * @brief Get human-readable text name of a `Field`.
 */
template <Field> constexpr auto field_name() noexcept -> std::string_view;


namespace decode_internal {

template <Msg_Class msg_class, Field field, typename val_T = value_T<field>>
constexpr auto
decode(std::string_view s) noexcept -> aiscpp_result<val_T> {
  static_assert(has_field<msg_class, field>() ||
                    msg_class == Msg_Class::unknown,
                AISCPP_BAD_FIELD);
  if constexpr (std::is_arithmetic_v<val_T>) {
    return decode_num<msg_class, field, val_T>(s);
  }
}

/**
 * Handle fields that must be decoded unsigned before final cast... le sigh
 */
template <Msg_Class msg_class, Field field, typename val_T = value_T<field>>
constexpr auto
decode_unsigned_first(std::string_view s) noexcept -> aiscpp_result<val_T> {
  static_assert(has_field<msg_class, field>() ||
                msg_class == Msg_Class::unknown);
  if constexpr (std::is_arithmetic_v<val_T>) {
    const auto [raw, status] = decode_num<msg_class, field, std::uint32_t>(s);
    return {static_cast<val_T>(raw), status};
  }
}


} // namespace decode_internal


/** @name Universal Fields
 *
 * @brief Data fields/members that are universal to all message types.
 *
 */
/*@{*/

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> msg_id
template <> struct Result_Value<Field::msg_id> {
  using type = typename std::uint32_t;
};

template <>
constexpr auto
field_name<Field::msg_id>() noexcept -> std::string_view {
  return "msg_id";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
msg_id(std::string_view s) noexcept -> aiscpp_result<value_T<Field::msg_id>> {
  return decode_internal::decode<msg_class, Field::msg_id>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::msg_id>() == "msg_id");

static_assert(msg_id(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_2_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_4_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_5_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_6_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_7_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_8_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_9_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_10_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_11_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_12_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_13_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_14_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_15_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_16_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_17_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_20_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_21_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_22_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_23_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_24_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_25_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_26_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_27_a).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_27_b).status() == status_code::SUCCESS);
static_assert(msg_id(static_data::pl_27_c).status() == status_code::SUCCESS);

static_assert(msg_id(static_data::pl_1_a) == 1);
static_assert(msg_id(static_data::pl_1_b) == 1);
static_assert(msg_id(static_data::pl_2_a) == 2);
static_assert(msg_id(static_data::pl_3_a) == 3);
static_assert(msg_id(static_data::pl_4_a) == 4);
static_assert(msg_id(static_data::pl_5_a) == 5);
static_assert(msg_id(static_data::pl_6_a) == 6);
static_assert(msg_id(static_data::pl_7_a) == 7);
static_assert(msg_id(static_data::pl_8_a) == 8);
static_assert(msg_id(static_data::pl_9_a) == 9);
static_assert(msg_id(static_data::pl_10_a) == 10);
static_assert(msg_id(static_data::pl_11_a) == 11);
static_assert(msg_id(static_data::pl_12_a) == 12);
static_assert(msg_id(static_data::pl_13_a) == 13);
static_assert(msg_id(static_data::pl_14_a) == 14);
static_assert(msg_id(static_data::pl_15_a) == 15);
static_assert(msg_id(static_data::pl_16_a) == 16);
static_assert(msg_id(static_data::pl_17_a) == 17);
static_assert(msg_id(static_data::pl_18_a) == 18);
static_assert(msg_id(static_data::pl_19_a) == 19);
static_assert(msg_id(static_data::pl_20_a) == 20);
static_assert(msg_id(static_data::pl_21_a) == 21);
static_assert(msg_id(static_data::pl_22_a) == 22);
static_assert(msg_id(static_data::pl_23_a) == 23);
static_assert(msg_id(static_data::pl_24_a) == 24);
static_assert(msg_id(static_data::pl_25_a) == 25);
static_assert(msg_id(static_data::pl_26_a) == 26);
static_assert(msg_id(static_data::pl_27_a) == 27);
static_assert(msg_id(static_data::pl_27_b) == 27);
static_assert(msg_id(static_data::pl_27_c) == 27);

static_assert(msg_id<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 1);
static_assert(msg_id<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 1);
static_assert(msg_id<Msg_Class::m_1_2_3>(static_data::pl_2_a) == 2);
static_assert(msg_id<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 3);
static_assert(msg_id<Msg_Class::m_4_11>(static_data::pl_4_a) == 4);
static_assert(msg_id<Msg_Class::m_5>(static_data::pl_5_a) == 5);
static_assert(msg_id<Msg_Class::m_6>(static_data::pl_6_a) == 6);
static_assert(msg_id<Msg_Class::m_7_13>(static_data::pl_7_a) == 7);
static_assert(msg_id<Msg_Class::m_8>(static_data::pl_8_a) == 8);
static_assert(msg_id<Msg_Class::m_9>(static_data::pl_9_a) == 9);
static_assert(msg_id<Msg_Class::m_10>(static_data::pl_10_a) == 10);
static_assert(msg_id<Msg_Class::m_4_11>(static_data::pl_11_a) == 11);
static_assert(msg_id<Msg_Class::m_12>(static_data::pl_12_a) == 12);
static_assert(msg_id<Msg_Class::m_7_13>(static_data::pl_13_a) == 13);
static_assert(msg_id<Msg_Class::m_14>(static_data::pl_14_a) == 14);
static_assert(msg_id<Msg_Class::m_15>(static_data::pl_15_a) == 15);
static_assert(msg_id<Msg_Class::m_16>(static_data::pl_16_a) == 16);
static_assert(msg_id<Msg_Class::m_17>(static_data::pl_17_a) == 17);
static_assert(msg_id<Msg_Class::m_18>(static_data::pl_18_a) == 18);
static_assert(msg_id<Msg_Class::m_19>(static_data::pl_19_a) == 19);
static_assert(msg_id<Msg_Class::m_20>(static_data::pl_20_a) == 20);
static_assert(msg_id<Msg_Class::m_21>(static_data::pl_21_a) == 21);
static_assert(msg_id<Msg_Class::m_22>(static_data::pl_22_a) == 22);
static_assert(msg_id<Msg_Class::m_23>(static_data::pl_23_a) == 23);
static_assert(msg_id<Msg_Class::m_24>(static_data::pl_24_a) == 24);
static_assert(msg_id<Msg_Class::m_25>(static_data::pl_25_a) == 25);
static_assert(msg_id<Msg_Class::m_26>(static_data::pl_26_a) == 26);
static_assert(msg_id<Msg_Class::m_27>(static_data::pl_27_a) == 27);
static_assert(msg_id<Msg_Class::m_27>(static_data::pl_27_b) == 27);
static_assert(msg_id<Msg_Class::m_27>(static_data::pl_27_c) == 27);
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< msg_id


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> repeat_indicator
template <> struct Result_Value<Field::repeat_indicator> {
  using type = typename std::uint32_t;
};

template <>
constexpr auto
field_name<Field::repeat_indicator>() noexcept -> std::string_view {
  return "repeat_indicator";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
repeat_indicator(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::repeat_indicator>> {
  return decode_internal::decode<msg_class, Field::repeat_indicator>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::repeat_indicator>() == "repeat_indicator");
// clang-format off
static_assert(repeat_indicator(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_2_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_4_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_5_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_6_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_7_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_8_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_9_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_10_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_11_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_12_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_13_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_14_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_15_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_16_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_17_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_20_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_21_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_22_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_23_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_24_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_25_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_26_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_27_a).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_27_b).status() == status_code::SUCCESS);
static_assert(repeat_indicator(static_data::pl_27_c).status() == status_code::SUCCESS);
// clang-format on
static_assert(repeat_indicator(static_data::pl_1_a) == 0);
static_assert(repeat_indicator(static_data::pl_1_b) == 0);
static_assert(repeat_indicator(static_data::pl_2_a) == 2);
static_assert(repeat_indicator(static_data::pl_3_a) == 0);
static_assert(repeat_indicator(static_data::pl_4_a) == 0);
static_assert(repeat_indicator(static_data::pl_5_a) == 0);
static_assert(repeat_indicator(static_data::pl_6_a) == 0);
static_assert(repeat_indicator(static_data::pl_7_a) == 0);
static_assert(repeat_indicator(static_data::pl_8_a) == 0);
static_assert(repeat_indicator(static_data::pl_9_a) == 3);
static_assert(repeat_indicator(static_data::pl_10_a) == 0);
static_assert(repeat_indicator(static_data::pl_11_a) == 0);
static_assert(repeat_indicator(static_data::pl_12_a) == 3);
static_assert(repeat_indicator(static_data::pl_13_a) == 0);
static_assert(repeat_indicator(static_data::pl_14_a) == 1);
static_assert(repeat_indicator(static_data::pl_15_a) == 0);
static_assert(repeat_indicator(static_data::pl_16_a) == 3);
static_assert(repeat_indicator(static_data::pl_17_a) == 3);
static_assert(repeat_indicator(static_data::pl_18_a) == 1);
static_assert(repeat_indicator(static_data::pl_19_a) == 0);
static_assert(repeat_indicator(static_data::pl_20_a) == 0);
static_assert(repeat_indicator(static_data::pl_21_a) == 0);
static_assert(repeat_indicator(static_data::pl_22_a) == 0);
static_assert(repeat_indicator(static_data::pl_23_a) == 0);
static_assert(repeat_indicator(static_data::pl_24_a) == 0);
static_assert(repeat_indicator(static_data::pl_25_a) == 0);
static_assert(repeat_indicator(static_data::pl_26_a) == 1);
static_assert(repeat_indicator(static_data::pl_27_a) == 3);
static_assert(repeat_indicator(static_data::pl_27_b) == 3);
static_assert(repeat_indicator(static_data::pl_27_c) == 3);

static_assert(repeat_indicator<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 0);
static_assert(repeat_indicator<Msg_Class::m_1_2_3>(static_data::pl_2_a) == 2);
static_assert(repeat_indicator<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_4_11>(static_data::pl_4_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_5>(static_data::pl_5_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_6>(static_data::pl_6_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_7_13>(static_data::pl_7_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_8>(static_data::pl_8_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_9>(static_data::pl_9_a) == 3);
static_assert(repeat_indicator<Msg_Class::m_10>(static_data::pl_10_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_4_11>(static_data::pl_11_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_12>(static_data::pl_12_a) == 3);
static_assert(repeat_indicator<Msg_Class::m_7_13>(static_data::pl_13_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_14>(static_data::pl_14_a) == 1);
static_assert(repeat_indicator<Msg_Class::m_15>(static_data::pl_15_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_16>(static_data::pl_16_a) == 3);
static_assert(repeat_indicator<Msg_Class::m_17>(static_data::pl_17_a) == 3);
static_assert(repeat_indicator<Msg_Class::m_18>(static_data::pl_18_a) == 1);
static_assert(repeat_indicator<Msg_Class::m_19>(static_data::pl_19_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_20>(static_data::pl_20_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_21>(static_data::pl_21_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_22>(static_data::pl_22_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_23>(static_data::pl_23_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_24>(static_data::pl_24_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_25>(static_data::pl_25_a) == 0);
static_assert(repeat_indicator<Msg_Class::m_26>(static_data::pl_26_a) == 1);
static_assert(repeat_indicator<Msg_Class::m_27>(static_data::pl_27_a) == 3);
static_assert(repeat_indicator<Msg_Class::m_27>(static_data::pl_27_b) == 3);
static_assert(repeat_indicator<Msg_Class::m_27>(static_data::pl_27_c) == 3);
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< repeat_indicator


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> mmsi
template <> struct Result_Value<Field::mmsi> {
  using type = typename std::uint32_t;
};

template <>
constexpr auto
field_name<Field::mmsi>() noexcept -> std::string_view {
  return "mmsi";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
mmsi(std::string_view s) noexcept -> aiscpp_result<value_T<Field::mmsi>> {
  return decode_internal::decode<msg_class, Field::mmsi>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::mmsi>() == "mmsi");

static_assert(mmsi(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_2_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_4_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_5_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_6_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_7_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_8_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_9_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_10_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_11_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_12_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_13_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_14_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_15_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_16_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_17_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_20_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_21_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_22_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_23_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_24_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_25_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_26_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_27_a).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_27_b).status() == status_code::SUCCESS);
static_assert(mmsi(static_data::pl_27_c).status() == status_code::SUCCESS);

static_assert(mmsi(static_data::pl_1_a) == 477553000);
static_assert(mmsi(static_data::pl_1_b) == 367033650);
static_assert(mmsi(static_data::pl_2_a) == 277315048);
static_assert(mmsi(static_data::pl_3_a) == 366268061);
static_assert(mmsi(static_data::pl_4_a) == 3669713);
static_assert(mmsi(static_data::pl_5_a) == 249190000);
static_assert(mmsi(static_data::pl_6_a) == 3669971);
static_assert(mmsi(static_data::pl_7_a) == 85876707);
static_assert(mmsi(static_data::pl_8_a) == 3669739);
static_assert(mmsi(static_data::pl_9_a) == 400214919);
static_assert(mmsi(static_data::pl_10_a) == 355696000);
static_assert(mmsi(static_data::pl_11_a) == 563716000);
static_assert(mmsi(static_data::pl_12_a) == 704636231);
static_assert(mmsi(static_data::pl_13_a) == 538003422);
static_assert(mmsi(static_data::pl_14_a) == 259038701);
static_assert(mmsi(static_data::pl_15_a) == 367036840);
static_assert(mmsi(static_data::pl_16_a) == 557529062);
static_assert(mmsi(static_data::pl_17_a) == 788513886);
static_assert(mmsi(static_data::pl_18_a) == 338085238);
static_assert(mmsi(static_data::pl_19_a) == 366943250);
static_assert(mmsi(static_data::pl_20_a) == 3100051);
static_assert(mmsi(static_data::pl_21_a) == 316011284);
static_assert(mmsi(static_data::pl_22_a) == 3160075);
static_assert(mmsi(static_data::pl_23_a) == 161024507);
static_assert(mmsi(static_data::pl_24_a) == 367399570);
static_assert(mmsi(static_data::pl_25_a) == 366999601);
static_assert(mmsi(static_data::pl_26_a) == 424664312);
static_assert(mmsi(static_data::pl_27_a) == 974291422);
static_assert(mmsi(static_data::pl_27_b) == 980900281);
static_assert(mmsi(static_data::pl_27_c) == 606631198);

static_assert(mmsi<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 477553000);
static_assert(mmsi<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 367033650);
static_assert(mmsi<Msg_Class::m_1_2_3>(static_data::pl_2_a) == 277315048);
static_assert(mmsi<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 366268061);
static_assert(mmsi<Msg_Class::m_4_11>(static_data::pl_4_a) == 3669713);
static_assert(mmsi<Msg_Class::m_5>(static_data::pl_5_a) == 249190000);
static_assert(mmsi<Msg_Class::m_6>(static_data::pl_6_a) == 3669971);
static_assert(mmsi<Msg_Class::m_6>(static_data::pl_7_a) == 85876707);
static_assert(mmsi<Msg_Class::m_8>(static_data::pl_8_a) == 3669739);
static_assert(mmsi<Msg_Class::m_9>(static_data::pl_9_a) == 400214919);
static_assert(mmsi<Msg_Class::m_10>(static_data::pl_10_a) == 355696000);
static_assert(mmsi<Msg_Class::m_4_11>(static_data::pl_11_a) == 563716000);
static_assert(mmsi<Msg_Class::m_12>(static_data::pl_12_a) == 704636231);
static_assert(mmsi<Msg_Class::m_7_13>(static_data::pl_13_a) == 538003422);
static_assert(mmsi<Msg_Class::m_14>(static_data::pl_14_a) == 259038701);
static_assert(mmsi<Msg_Class::m_15>(static_data::pl_15_a) == 367036840);
static_assert(mmsi<Msg_Class::m_16>(static_data::pl_16_a) == 557529062);
static_assert(mmsi<Msg_Class::m_17>(static_data::pl_17_a) == 788513886);
static_assert(mmsi<Msg_Class::m_18>(static_data::pl_18_a) == 338085238);
static_assert(mmsi<Msg_Class::m_19>(static_data::pl_19_a) == 366943250);
static_assert(mmsi<Msg_Class::m_20>(static_data::pl_20_a) == 3100051);
static_assert(mmsi<Msg_Class::m_21>(static_data::pl_21_a) == 316011284);
static_assert(mmsi<Msg_Class::m_22>(static_data::pl_22_a) == 3160075);
static_assert(mmsi<Msg_Class::m_23>(static_data::pl_23_a) == 161024507);
static_assert(mmsi<Msg_Class::m_24>(static_data::pl_24_a) == 367399570);
static_assert(mmsi<Msg_Class::m_25>(static_data::pl_25_a) == 366999601);
static_assert(mmsi<Msg_Class::m_26>(static_data::pl_26_a) == 424664312);
static_assert(mmsi<Msg_Class::m_27>(static_data::pl_27_a) == 974291422);
static_assert(mmsi<Msg_Class::m_27>(static_data::pl_27_b) == 980900281);
static_assert(mmsi<Msg_Class::m_27>(static_data::pl_27_c) == 606631198);
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< mmsi


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> nav_status
template <> struct Result_Value<Field::nav_status> {
  using type = typename std::uint32_t;
};

template <>
constexpr auto
field_name<Field::nav_status>() noexcept -> std::string_view {
  return "nav_status";
}
static_assert(field_name<Field::nav_status>() == "nav_status");

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
nav_status(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::nav_status>> {
  return decode_internal::decode<msg_class, Field::nav_status>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::nav_status>() == "nav_status");
// clang-format off
static_assert(nav_status(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(nav_status(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(nav_status(static_data::pl_2_a).status() == status_code::SUCCESS);
static_assert(nav_status(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(nav_status(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_18_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_19_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(nav_status(static_data::pl_27_a).status() == status_code::SUCCESS);
static_assert(nav_status(static_data::pl_27_b).status() == status_code::SUCCESS);
static_assert(nav_status(static_data::pl_27_c).status() == status_code::SUCCESS);

static_assert(nav_status(static_data::pl_1_a) == 5);
static_assert(nav_status(static_data::pl_1_b) == 0);
static_assert(nav_status(static_data::pl_2_a) == 14);
static_assert(nav_status(static_data::pl_3_a) == 0);
static_assert(nav_status(static_data::pl_27_a) == 0);
static_assert(nav_status(static_data::pl_27_b) == 13);
static_assert(nav_status(static_data::pl_27_c) == 8);

static_assert(nav_status<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 0);
static_assert(nav_status<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 5);
static_assert(nav_status<Msg_Class::m_1_2_3>(static_data::pl_2_a) == 14);
static_assert(nav_status<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 0);
static_assert(nav_status<Msg_Class::m_27>(static_data::pl_27_a) == 0);
static_assert(nav_status<Msg_Class::m_27>(static_data::pl_27_b) == 13);
static_assert(nav_status<Msg_Class::m_27>(static_data::pl_27_c) == 8);
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< nav_status


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> rate_of_turn
template <> struct Result_Value<Field::rate_of_turn> { using type = double; };

template <>
constexpr auto
field_name<Field::rate_of_turn>() noexcept -> std::string_view {
  return "rate_of_turn";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
rate_of_turn(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::rate_of_turn>> {
  constexpr value_T<Field::rate_of_turn> factor = 1.0 / 4.733;
  const auto [rot_raw, status] =
      decode_internal::decode<msg_class, Field::rate_of_turn>(s);
  const auto res = static_cast<value_T<Field::rate_of_turn>>(rot_raw) * factor;
  return {res * (res >= 0 ? res : -res), status}; // square res, but keep sign
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::rate_of_turn>() == "rate_of_turn");
// clang-format off
static_assert(rate_of_turn(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(rate_of_turn(static_data::pl_1_b).status() == status_code::NOT_AVAILABLE);
static_assert(rate_of_turn(static_data::pl_2_a).status() == status_code::SUCCESS);
static_assert(rate_of_turn(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(rate_of_turn(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_18_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_19_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_27_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_27_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(rate_of_turn(static_data::pl_27_c).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);

#if AISCPP_ENABLE_STATIC_TESTS_FLOATING_POINT
static_assert(rate_of_turn(static_data::pl_1_a) == 0);
static_assert(rate_of_turn(static_data::pl_1_b) == -(128.0 / 4.733 * 128.0 / 4.733)); // precision retained when multiplying reciprocal (instead of dividing)
static_assert(rate_of_turn(static_data::pl_2_a) == -(31.0 / 4.733 * 31.0 / 4.733));
static_assert(rate_of_turn(static_data::pl_3_a) == 0);

static_assert(rate_of_turn<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 0);
static_assert(rate_of_turn<Msg_Class::m_1_2_3>(static_data::pl_1_b) == -(128.0 / 4.733 * 128.0 / 4.733));
static_assert(rate_of_turn<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 0);
#endif
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< rate_of_turn


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> speed_over_ground
template <> struct Result_Value<Field::speed_over_ground> {
  using type = double;
};

template <>
constexpr auto
field_name<Field::speed_over_ground>() noexcept -> std::string_view {
  return "speed_over_ground";
}


template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
speed_over_ground(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::speed_over_ground>> {
  constexpr auto SOG                        = Field::speed_over_ground;
  using sog_T                               = value_T<SOG>;
  constexpr std::uint32_t exceeds_101_2_val = 1023;       // all but m_27
  constexpr sog_T         factor            = 1.0 / 10.0; // all but m_27

  if constexpr (msg_class == Msg_Class::unknown) {
    switch (s[0]) {
      case 'K': // m_27
        return decode_internal::decode_unsigned_first<msg_class, SOG, sog_T>(s);

      default: {
        const auto [sog_raw, status] =
            decode_internal::decode_unsigned_first<msg_class, SOG>(s);
        return {static_cast<sog_T>(sog_raw) * factor,
                sog_raw == exceeds_101_2_val
                    ? status_code::SPEED_OVER_GROUND_EXCEEDS_102_POINT_2_KNOTS
                    : status};
      }
    } // switch (s[0])

  } else { // msg_class != Msg_Class::unknown
    if constexpr (msg_class == Msg_Class::m_27) {
      return decode_internal::decode_unsigned_first<msg_class, SOG, sog_T>(s);

    } else { // msg_class != Msg_Class::m_27
      const auto [sog_raw, status] =
          decode_internal::decode_unsigned_first<msg_class,
                                                 Field::speed_over_ground>(s);
      return {static_cast<sog_T>(sog_raw) * factor,
              sog_raw == exceeds_101_2_val
                  ? status_code::SPEED_OVER_GROUND_EXCEEDS_102_POINT_2_KNOTS
                  : status};
    }
  }
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::speed_over_ground>() == "speed_over_ground");
// clang-format off
static_assert(speed_over_ground(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(speed_over_ground(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(speed_over_ground(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(speed_over_ground(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(speed_over_ground(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(speed_over_ground(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(speed_over_ground(static_data::pl_27_a).status() == status_code::SUCCESS);
static_assert(speed_over_ground(static_data::pl_27_b).status() == status_code::SUCCESS);
static_assert(speed_over_ground(static_data::pl_27_c).status() == status_code::SUCCESS);

#if AISCPP_ENABLE_STATIC_TESTS_FLOATING_POINT
static_assert(speed_over_ground(static_data::pl_1_a) == 0.0);
static_assert(speed_over_ground(static_data::pl_1_b) == 8.6);
static_assert(speed_over_ground(static_data::pl_18_a) == 0.0);
static_assert(speed_over_ground(static_data::pl_19_a) == 0.1);
static_assert(speed_over_ground(static_data::pl_27_a) == 16.0);
static_assert(speed_over_ground(static_data::pl_27_b) == 35.0);
static_assert(speed_over_ground(static_data::pl_27_c) == 50.0);

static_assert(speed_over_ground<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 0.0);
static_assert(speed_over_ground<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 8.6);
static_assert(speed_over_ground<Msg_Class::m_18>(static_data::pl_18_a) == 0.0);
static_assert(speed_over_ground<Msg_Class::m_19>(static_data::pl_19_a) == 0.1);
static_assert(speed_over_ground<Msg_Class::m_27>(static_data::pl_27_a) == 16.0);
static_assert(speed_over_ground<Msg_Class::m_27>(static_data::pl_27_b) == 35.0);
static_assert(speed_over_ground<Msg_Class::m_27>(static_data::pl_27_c) == 50.0);
#endif
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< speed_over_ground


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> position_accuracy
template <> struct Result_Value<Field::position_accuracy> {
  using type = bool;
};

template <>
constexpr auto
field_name<Field::position_accuracy>() noexcept -> std::string_view {
  return "position_accuracy";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
position_accuracy(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::position_accuracy>> {
  return decode_internal::decode<msg_class, Field::position_accuracy>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::position_accuracy>() == "position_accuracy");
// clang-format off
static_assert(position_accuracy(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(position_accuracy(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(position_accuracy(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(position_accuracy(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_18_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_19_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_27_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_27_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(position_accuracy(static_data::pl_27_c).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);

static_assert(!position_accuracy(static_data::pl_1_a));
static_assert(!position_accuracy(static_data::pl_1_b));
static_assert(!position_accuracy(static_data::pl_3_a));

static_assert(!position_accuracy<Msg_Class::m_1_2_3>(static_data::pl_1_a));
static_assert(!position_accuracy<Msg_Class::m_1_2_3>(static_data::pl_1_b));
static_assert(!position_accuracy<Msg_Class::m_1_2_3>(static_data::pl_3_a));
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< position_accuracy


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> longitude
template <> struct Result_Value<Field::longitude> { using type = double; };

template <>
constexpr auto
field_name<Field::longitude>() noexcept -> std::string_view {
  return "longitude";
}


template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
longitude(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::longitude>> {
  constexpr auto FIELD = Field::longitude;

  if constexpr (msg_class == Msg_Class::unknown) {
    switch (const auto [raw_lat, status] =
                decode_internal::decode<msg_class, FIELD>(s);
            static_cast<Msg_ID>(s[0])) {
      // case Msg_ID::i_9:
      //   [[fallthrough]];
      case Msg_ID::i_17:
        [[fallthrough]];
      case Msg_ID::i_27: {
        constexpr value_T<FIELD> divisor = 600.0;
        return {raw_lat / divisor, status};
      }

      default: {
        constexpr value_T<FIELD> factor = 1 / 600'000.0;
        return {raw_lat * factor, status};
      }
    }

  } else { // msg_class != Msg_Class::unknown
    const auto [raw_lat, status] = decode_internal::decode<msg_class, FIELD>(s);
    if constexpr (msg_class == Msg_Class::m_17 ||
                  msg_class == Msg_Class::m_27) {
      constexpr value_T<FIELD> divisor = 600.0;
      return {raw_lat / divisor, status};
    } else {
      constexpr value_T<FIELD> factor = 1.0 / 600'000.0;
      return {raw_lat * factor, status};
    }
  }
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::longitude>() == "longitude");
// clang-format off
static_assert(longitude(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(longitude(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(longitude(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(longitude(static_data::pl_4_a).status() == status_code::NOT_AVAILABLE);
static_assert(longitude(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_9_a).status() == status_code::SUCCESS);
static_assert(longitude(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_11_a).status() == status_code::SUCCESS);
static_assert(longitude(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_17_a).status() == status_code::SUCCESS);
static_assert(longitude(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(longitude(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(longitude(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_21_a).status() == status_code::SUCCESS);
static_assert(longitude(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(longitude(static_data::pl_27_a).status() == status_code::NOT_DEFINED);
static_assert(longitude(static_data::pl_27_b).status() == status_code::SUCCESS);
static_assert(longitude(static_data::pl_27_c).status() == status_code::SUCCESS);

#if AISCPP_ENABLE_STATIC_TESTS_FLOATING_POINT
static_assert(longitude(static_data::pl_1_a) == -73407500.0 / 600'000.0);
static_assert(longitude(static_data::pl_1_b) == -42208143.0 / 600'000.0);
static_assert(longitude(static_data::pl_2_a) == -12.425178333333333);
static_assert(longitude(static_data::pl_3_a) == -93.96876833333333);
static_assert(longitude(static_data::pl_4_a) == 108600000.0 / 600'000);
static_assert(longitude(static_data::pl_9_a) == 60.96993666666667);
static_assert(longitude(static_data::pl_11_a) == -55601170.0 / 600'000.0);
static_assert(longitude(static_data::pl_17_a) == -91.91833333333334);
static_assert(longitude(static_data::pl_18_a) == -73677745.0 / 600'000);
static_assert(longitude(static_data::pl_19_a) == -52826080.0 / 600'000.0);
static_assert(longitude(static_data::pl_21_a) == -73823231.0 / 600'000.0);
static_assert(longitude(static_data::pl_27_a) == 120139.0 / 600.0);
static_assert(longitude(static_data::pl_27_b) == -16675.0 / 600.0);
static_assert(longitude(static_data::pl_27_c) == 39547.0 / 600.0);

static_assert(longitude<Msg_Class::m_1_2_3>(static_data::pl_1_a) == -73407500.0 / 600'000.0);
static_assert(longitude<Msg_Class::m_1_2_3>(static_data::pl_1_b) == -42208143.0 / 600'000.0);
static_assert(longitude<Msg_Class::m_1_2_3>(static_data::pl_2_a) == -12.425178333333333);
static_assert(longitude<Msg_Class::m_1_2_3>(static_data::pl_3_a) == -93.96876833333333);
static_assert(longitude<Msg_Class::m_4_11>(static_data::pl_4_a) == 181.0); // status_code::NOT_AVAILABLE
static_assert(longitude<Msg_Class::m_9>(static_data::pl_9_a).value() == 60.96993666666667);
static_assert(longitude<Msg_Class::m_4_11>(static_data::pl_11_a) == -55601170.0 / 600'000.0);
static_assert(longitude<Msg_Class::m_17>(static_data::pl_17_a) == -55151000.0 / 600'000.0);
static_assert(longitude<Msg_Class::m_18>(static_data::pl_18_a) == -73677745.0 / 600'000);
static_assert(longitude<Msg_Class::m_19>(static_data::pl_19_a) == -52826080.0 / 600'000.0);
static_assert(longitude<Msg_Class::m_21>(static_data::pl_21_a) == -73823231.0 / 600'000.0);
static_assert(longitude<Msg_Class::m_27>(static_data::pl_27_a) == 120139.0 / 600.0);
static_assert(longitude<Msg_Class::m_27>(static_data::pl_27_a).status() == status_code::NOT_DEFINED);
static_assert(longitude<Msg_Class::m_27>(static_data::pl_27_b) == -16675.0 / 600.0);
static_assert(longitude<Msg_Class::m_27>(static_data::pl_27_c) == 39547.0 / 600.0);
#endif
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< longitude


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> latitude
template <> struct Result_Value<Field::latitude> { using type = double; };

template <>
constexpr auto
field_name<Field::latitude>() noexcept -> std::string_view {
  return "latitude";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
latitude(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::latitude>> {
  constexpr auto FIELD = Field::latitude;

  if constexpr (msg_class == Msg_Class::unknown) {
    switch (const auto [raw_lat, status] =
                decode_internal::decode<msg_class, FIELD>(s);
            get_msg_id(s)) {
      case Msg_ID::i_17:
        [[fallthrough]];
      case Msg_ID::i_27: {
        constexpr value_T<FIELD> divisor = 600.0;
        return {raw_lat / divisor, status};
      }

      default: {
        constexpr value_T<FIELD> factor = 1 / 600'000.0;
        return {raw_lat * factor, status};
      }
    }

  } else { // msg_class != Msg_Class::unknown
    const auto [raw_lat, status] = decode_internal::decode<msg_class, FIELD>(s);
    if constexpr (msg_class == Msg_Class::m_17 ||
                  msg_class == Msg_Class::m_27) {
      constexpr value_T<FIELD> divisor = 600.0;
      return {raw_lat / divisor, status};
    } else {
      constexpr value_T<FIELD> factor = 1.0 / 600'000.0;
      return {raw_lat * factor, status};
    }
  }
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::latitude>() == "latitude");
// clang-format off
static_assert(latitude(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(latitude(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(latitude(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(latitude(static_data::pl_4_a).status() == status_code::NOT_AVAILABLE);
static_assert(latitude(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_9_a).status() == status_code::NOT_DEFINED);
static_assert(latitude(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_11_a).status() == status_code::SUCCESS);
static_assert(latitude(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_17_a).status() == status_code::SUCCESS);
static_assert(latitude(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(latitude(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(latitude(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_21_a).status() == status_code::SUCCESS);
static_assert(latitude(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(latitude(static_data::pl_27_a).status() == status_code::SUCCESS);
static_assert(latitude(static_data::pl_27_b).status() == status_code::SUCCESS);
static_assert(latitude(static_data::pl_27_c).status() == status_code::SUCCESS);

#if AISCPP_ENABLE_STATIC_TESTS_FLOATING_POINT
static_assert(latitude(static_data::pl_1_a) == 28'549'700.0 / 600'000.0); // 47.58283333333333
static_assert(latitude(static_data::pl_1_b) == 25679130.0 / 600'000.0); // 42.79855
static_assert(latitude(static_data::pl_2_a) == -101.54704166666667);
static_assert(latitude(static_data::pl_3_a) == 29.841335);
static_assert(latitude(static_data::pl_4_a) == 91.0);
static_assert(latitude(static_data::pl_9_a) == -99.42364666666667); // status_code::NOT_DEFINED
static_assert(latitude(static_data::pl_11_a) == 27.5552);
static_assert(latitude(static_data::pl_17_a) == 38.501666666666665);
static_assert(latitude(static_data::pl_18_a) == 45.86612);
static_assert(latitude(static_data::pl_19_a) == 30.708233333333332);
static_assert(latitude(static_data::pl_21_a) == 49.148651666666666);
static_assert(latitude(static_data::pl_27_a) == -77.96833333333333);
static_assert(latitude(static_data::pl_27_b) == -15.033333333333333);
static_assert(latitude(static_data::pl_27_c) == -43.818333333333335);

static_assert(latitude<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 47.58283333333333);
static_assert(latitude<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 42.79855);
static_assert(latitude<Msg_Class::m_1_2_3>(static_data::pl_2_a) == -101.54704166666667);
static_assert(latitude<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 29.841335);
static_assert(latitude<Msg_Class::m_4_11>(static_data::pl_4_a) == 91.0);
static_assert(latitude<Msg_Class::m_9>(static_data::pl_9_a) == -99.42364666666667); // status_code::NOT_DEFINED
static_assert(latitude<Msg_Class::m_4_11>(static_data::pl_11_a) == 27.5552);
static_assert(latitude<Msg_Class::m_17>(static_data::pl_17_a) == 38.501666666666665);
static_assert(latitude<Msg_Class::m_18>(static_data::pl_18_a) == 45.86612);
static_assert(latitude<Msg_Class::m_19>(static_data::pl_19_a) == 30.708233333333332);
static_assert(latitude<Msg_Class::m_21>(static_data::pl_21_a) == 49.148651666666666);
static_assert(latitude<Msg_Class::m_27>(static_data::pl_27_a) == -77.96833333333333);
static_assert(latitude<Msg_Class::m_27>(static_data::pl_27_b) == -15.033333333333333);
static_assert(latitude<Msg_Class::m_27>(static_data::pl_27_c) == -43.818333333333335);
#endif
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< latitude


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> course_over_ground
template <> struct Result_Value<Field::course_over_ground> {
  using type = double;
};

template <>
constexpr auto
field_name<Field::course_over_ground>() noexcept -> std::string_view {
  return "course_over_ground";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
course_over_ground(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::course_over_ground>> {
  constexpr auto           FIELD   = Field::course_over_ground;
  constexpr value_T<FIELD> divisor = 10.0;

  if constexpr (msg_class == Msg_Class::unknown) {
    switch (s[0]) {
      // case '4': return status_code::MSG_TYPE_HAS_NO_SUCH_FIELD;
      case 'K': // m_27
        return decode_internal::decode_unsigned_first<Msg_Class::m_27, FIELD>(
            s);
      default: {
        const auto [res, status] =
            decode_internal::decode_unsigned_first<msg_class, FIELD>(s);
        return {res / divisor, status};
      }
    }

  } else { // msg_class != Msg_Class::unknown
    if constexpr (msg_class == Msg_Class::m_27) {
      return decode_internal::decode_unsigned_first<msg_class, FIELD>(s);
    } else {
      const auto [res, status] =
          decode_internal::decode_unsigned_first<msg_class, FIELD>(s);
      return {res / divisor, status};
    }
  }
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::course_over_ground>() == "course_over_ground");
// clang-format off
static_assert(course_over_ground(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(course_over_ground(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(course_over_ground(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(course_over_ground(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(course_over_ground(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(course_over_ground(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(course_over_ground(static_data::pl_27_a).status() == status_code::SUCCESS);
static_assert(course_over_ground(static_data::pl_27_b).status() == status_code::NOT_DEFINED);
static_assert(course_over_ground(static_data::pl_27_c).status() == status_code::SUCCESS);

#if AISCPP_ENABLE_STATIC_TESTS_FLOATING_POINT
static_assert(course_over_ground(static_data::pl_1_a) == 51.0);
static_assert(course_over_ground(static_data::pl_1_b) == 35.9);
static_assert(course_over_ground(static_data::pl_18_a) == 0.0);
static_assert(course_over_ground(static_data::pl_19_a).value_ == 37.8);
static_assert(course_over_ground(static_data::pl_27_a) == 255.0);
static_assert(course_over_ground(static_data::pl_27_b) == 444.0);
static_assert(course_over_ground(static_data::pl_27_b).status() == status_code::NOT_DEFINED);
static_assert(course_over_ground(static_data::pl_27_c) == 80.0);

static_assert(course_over_ground<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 51.0);
static_assert(course_over_ground<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 35.9);
static_assert(course_over_ground<Msg_Class::m_18>(static_data::pl_18_a) == 0.0);
static_assert(course_over_ground<Msg_Class::m_19>(static_data::pl_19_a) == 37.8);
static_assert(course_over_ground<Msg_Class::m_27>(static_data::pl_27_a) == 255.0);
static_assert(course_over_ground<Msg_Class::m_27>(static_data::pl_27_b) == 444.0);
static_assert(course_over_ground<Msg_Class::m_27>(static_data::pl_27_b).status() == status_code::NOT_DEFINED);
static_assert(course_over_ground<Msg_Class::m_27>(static_data::pl_27_c) == 80.0);
#endif
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< course_over_ground


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> true_heading
template <> struct Result_Value<Field::true_heading> {
  using type = std::uint32_t;
};

template <>
constexpr auto
field_name<Field::true_heading>() noexcept -> std::string_view {
  return "true_heading";
}
template <Msg_Class msg_class, Field field, typename val_T = value_T<field>>
constexpr auto decode(std::string_view s) noexcept -> aiscpp_result<val_T>;
template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
true_heading(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::true_heading>> {
  return decode_internal::decode<msg_class, Field::true_heading>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::true_heading>() == "true_heading");
// clang-format off
static_assert(true_heading(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(true_heading(static_data::pl_1_b).status() == status_code::NOT_AVAILABLE);
static_assert(true_heading(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(true_heading(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_18_a).status() == status_code::NOT_AVAILABLE);
static_assert(true_heading(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(true_heading(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_27_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_27_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(true_heading(static_data::pl_27_c).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);

static_assert(true_heading(static_data::pl_1_a) == 181);
static_assert(true_heading(static_data::pl_1_b) == 511);
static_assert(true_heading(static_data::pl_3_a) == 359);
static_assert(true_heading(static_data::pl_18_a) == 511);
static_assert(true_heading(static_data::pl_19_a) == 123);

static_assert(true_heading<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 181);
static_assert(true_heading<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 511);
static_assert(true_heading<Msg_Class::m_1_2_3>(static_data::pl_1_b).status() == status_code::NOT_AVAILABLE);
static_assert(true_heading<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 359);
static_assert(true_heading<Msg_Class::m_18>(static_data::pl_18_a) == 511);
static_assert(true_heading<Msg_Class::m_18>(static_data::pl_18_a).status() == status_code::NOT_AVAILABLE);
static_assert(true_heading<Msg_Class::m_19>(static_data::pl_19_a) == 123);
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< true_heading


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> timestamp
template <> struct Result_Value<Field::timestamp> {
  using type = std::uint32_t;
};

template <>
constexpr auto
field_name<Field::timestamp>() noexcept -> std::string_view {
  return "timestamp";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
timestamp(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::timestamp>> {
  switch (const auto [res, status] =
              decode_internal::decode<msg_class, Field::timestamp>(s);
          res) {
    case 61:
      return {res, status_code::POSITIONING_SYSTEM_IN_MANUAL_INPUT_MODE};
    case 62:
      return {
          res,
          status_code::ELECTRONIC_POSITION_FIXING_SYSTEMS_IN_ESTIMATED_MODE};
    case 63:
      return {res, status_code::POSITIONING_SYSTEM_INOPERATIVE};
    default:
      return {res, status};
  }
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::timestamp>() == "timestamp");
// clang-format off
static_assert(timestamp(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(timestamp(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(timestamp(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(timestamp(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_9_a).status() == status_code::SUCCESS);
static_assert(timestamp(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(timestamp(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(timestamp(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_27_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_27_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(timestamp(static_data::pl_27_c).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);

static_assert(timestamp(static_data::pl_1_a) == 15);
static_assert(timestamp(static_data::pl_1_b) == 56);
static_assert(timestamp(static_data::pl_3_a) == 0);
static_assert(timestamp(static_data::pl_9_a) == 31);
static_assert(timestamp(static_data::pl_18_a) == 20);
static_assert(timestamp(static_data::pl_19_a) == 52);

static_assert(timestamp<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 15);
static_assert(timestamp<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 56);
static_assert(timestamp<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 0);
static_assert(timestamp<Msg_Class::m_9>(static_data::pl_9_a) == 31);
static_assert(timestamp<Msg_Class::m_18>(static_data::pl_18_a) == 20);
static_assert(timestamp<Msg_Class::m_19>(static_data::pl_19_a) == 52);
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< timestamp


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> special_maneuver
template <> struct Result_Value<Field::special_maneuver> {
  using type = std::uint32_t;
};

template <>
constexpr auto
field_name<Field::special_maneuver>() noexcept -> std::string_view {
  return "special_maneuver";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
special_maneuver(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::special_maneuver>> {
  return decode_internal::decode<msg_class, Field::special_maneuver>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::special_maneuver>() == "special_maneuver");
// clang-format off
static_assert(special_maneuver(static_data::pl_1_a).status() == status_code::NOT_AVAILABLE);
static_assert(special_maneuver(static_data::pl_1_b).status() == status_code::NOT_AVAILABLE);
static_assert(special_maneuver(static_data::pl_2_a).status() == status_code::SUCCESS);
static_assert(special_maneuver(static_data::pl_3_a).status() == status_code::NOT_AVAILABLE);
static_assert(special_maneuver(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_18_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_19_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_27_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_27_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(special_maneuver(static_data::pl_27_c).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);

static_assert(special_maneuver(static_data::pl_1_a) == 0); // status_code::NOT_AVAILABLE
static_assert(special_maneuver(static_data::pl_1_b) == 0); // status_code::NOT_AVAILABLE
static_assert(special_maneuver(static_data::pl_2_a) == 2);
static_assert(special_maneuver(static_data::pl_3_a) == 0); // status_code::NOT_AVAILABLE

static_assert(special_maneuver<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 0);
static_assert(special_maneuver<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 0);
static_assert(special_maneuver<Msg_Class::m_1_2_3>(static_data::pl_2_a) == 2);
static_assert(special_maneuver<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 0);
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< special_maneuver


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> spare
template <> struct Result_Value<Field::spare> { using type = std::uint32_t; };

template <>
constexpr auto
field_name<Field::spare>() noexcept -> std::string_view {
  return "spare";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
spare(std::string_view s) noexcept -> aiscpp_result<value_T<Field::spare>> {
  return decode_internal::decode<msg_class, Field::spare>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::spare>() == "spare");
// clang-format off
static_assert(spare(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_4_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_5_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_6_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_7_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_8_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_9_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_10_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_11_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_12_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_13_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_14_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_15_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_16_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_17_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_20_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_21_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_22_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_23_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_24_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare(static_data::pl_27_a).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_27_b).status() == status_code::SUCCESS);
static_assert(spare(static_data::pl_27_c).status() == status_code::SUCCESS);

static_assert(spare(static_data::pl_1_a) == 0);
static_assert(spare(static_data::pl_1_b) == 0);
static_assert(spare(static_data::pl_3_a) == 0);
static_assert(spare(static_data::pl_4_a) == 0);
static_assert(spare(static_data::pl_5_a) == 0);
static_assert(spare(static_data::pl_6_a) == 0);
static_assert(spare(static_data::pl_7_a) == 3);
static_assert(spare(static_data::pl_8_a) == 0);
static_assert(spare(static_data::pl_9_a) == 83);
static_assert(spare(static_data::pl_10_a) == 0);
static_assert(spare(static_data::pl_11_a) == 0);
static_assert(spare(static_data::pl_12_a) == 0);
static_assert(spare(static_data::pl_13_a) == 0);
static_assert(spare(static_data::pl_14_a) == 1);
static_assert(spare(static_data::pl_15_a) == 0);
static_assert(spare(static_data::pl_16_a) == 3);
static_assert(spare(static_data::pl_17_a) == 3);
static_assert(spare(static_data::pl_18_a) == 0);
static_assert(spare(static_data::pl_19_a) == 216);
static_assert(spare(static_data::pl_20_a) == 0);
static_assert(spare(static_data::pl_21_a) == 0);
static_assert(spare(static_data::pl_22_a) == 0);
static_assert(spare(static_data::pl_23_a) == 3);
static_assert(spare(static_data::pl_24_a) == 0);
// m_25
// m_26
static_assert(spare(static_data::pl_27_a) == 1);
static_assert(spare(static_data::pl_27_b) == 0);
static_assert(spare(static_data::pl_27_c) == 0);

static_assert(spare<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 0);
static_assert(spare<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 0);
static_assert(spare<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 0);
static_assert(spare<Msg_Class::m_4_11>(static_data::pl_4_a) == 0);
static_assert(spare<Msg_Class::m_5>(static_data::pl_5_a) == 0);
static_assert(spare<Msg_Class::m_6>(static_data::pl_6_a) == 0);
static_assert(spare<Msg_Class::m_7_13>(static_data::pl_7_a) == 3);
static_assert(spare<Msg_Class::m_8>(static_data::pl_8_a) == 0);
static_assert(spare<Msg_Class::m_9>(static_data::pl_9_a) == 83);
static_assert(spare<Msg_Class::m_10>(static_data::pl_10_a) == 0);
static_assert(spare<Msg_Class::m_4_11>(static_data::pl_11_a) == 0);
static_assert(spare<Msg_Class::m_12>(static_data::pl_12_a) == 0);
static_assert(spare<Msg_Class::m_7_13>(static_data::pl_13_a) == 0);
static_assert(spare<Msg_Class::m_14>(static_data::pl_14_a) == 1);
static_assert(spare<Msg_Class::m_15>(static_data::pl_15_a) == 0);
static_assert(spare<Msg_Class::m_16>(static_data::pl_16_a) == 3);
static_assert(spare<Msg_Class::m_17>(static_data::pl_17_a) == 3);
static_assert(spare<Msg_Class::m_19>(static_data::pl_19_a) == 216);
static_assert(spare<Msg_Class::m_20>(static_data::pl_20_a) == 0);
static_assert(spare<Msg_Class::m_21>(static_data::pl_21_a) == 0);
static_assert(spare<Msg_Class::m_22>(static_data::pl_22_a) == 0);
static_assert(spare<Msg_Class::m_23>(static_data::pl_23_a) == 3);
static_assert(spare<Msg_Class::m_24>(static_data::pl_24_a) == 0);
// m_25
// m_26
static_assert(spare<Msg_Class::m_27>(static_data::pl_27_a) == 1);
static_assert(spare<Msg_Class::m_27>(static_data::pl_27_b) == 0);
static_assert(spare<Msg_Class::m_27>(static_data::pl_27_c) == 0);
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spare


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> spare2
template <> struct Result_Value<Field::spare2> { using type = std::uint32_t; };

template <>
constexpr auto
field_name<Field::spare2>() noexcept -> std::string_view {
  return "spare2";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
spare2(std::string_view s) noexcept -> aiscpp_result<value_T<Field::spare2>> {
  return decode_internal::decode<msg_class, Field::spare2>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::spare2>() == "spare2");
// clang-format off
static_assert(spare2(static_data::pl_1_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_1_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_3_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_9_a).status() == status_code::SUCCESS);
static_assert(spare2(static_data::pl_10_a).status() == status_code::SUCCESS);
static_assert(spare2(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_15_a).status() == status_code::SUCCESS);
static_assert(spare2(static_data::pl_16_a).status() == status_code::SUCCESS);
static_assert(spare2(static_data::pl_17_a).status() == status_code::SUCCESS);
static_assert(spare2(static_data::pl_18_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(spare2(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_22_a).status() == status_code::SUCCESS);
static_assert(spare2(static_data::pl_23_a).status() == status_code::SUCCESS);
static_assert(spare2(static_data::pl_24_a).status() == status_code::SUCCESS);
static_assert(spare2(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_27_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_27_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare2(static_data::pl_27_c).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);

static_assert(spare2(static_data::pl_9_a) == 1);
static_assert(spare2(static_data::pl_15_a) == 0);
static_assert(spare2(static_data::pl_16_a) == 2);
static_assert(spare2(static_data::pl_17_a) == 23);
static_assert(spare2(static_data::pl_19_a) == 12);
static_assert(spare2(static_data::pl_22_a) == 0);
static_assert(spare2(static_data::pl_23_a) == 939898);
static_assert(spare2(static_data::pl_24_a) == 0);

static_assert(spare2<Msg_Class::m_9>(static_data::pl_9_a) == 1);
static_assert(spare2<Msg_Class::m_15>(static_data::pl_15_a) == 0);
static_assert(spare2<Msg_Class::m_16>(static_data::pl_16_a) == 2);
static_assert(spare2<Msg_Class::m_17>(static_data::pl_17_a) == 23);
static_assert(spare2<Msg_Class::m_19>(static_data::pl_19_a) == 12);
static_assert(spare2<Msg_Class::m_22>(static_data::pl_22_a) == 0);
static_assert(spare2<Msg_Class::m_23>(static_data::pl_23_a) == 939898);
static_assert(spare2<Msg_Class::m_24>(static_data::pl_24_a) == 0);
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spare2


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> spare3
template <> struct Result_Value<Field::spare3> { using type = std::uint32_t; };

template <>
constexpr auto
field_name<Field::spare3>() noexcept -> std::string_view {
  return "spare3";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
spare3(std::string_view s) noexcept -> aiscpp_result<value_T<Field::spare3>> {
  return decode_internal::decode<msg_class, Field::spare3>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::spare3>() == "spare3");
// clang-format off
static_assert(spare3(static_data::pl_1_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_1_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_3_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_15_a).status() == status_code::SUCCESS);
static_assert(spare3(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_18_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_19_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_23_a).status() == status_code::SUCCESS);
static_assert(spare3(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_27_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_27_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare3(static_data::pl_27_c).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);

static_assert(spare3(static_data::pl_15_a) == 0);
static_assert(spare3(static_data::pl_23_a) == 55);

static_assert(spare3<Msg_Class::m_15>(static_data::pl_15_a) == 0);
static_assert(spare3<Msg_Class::m_23>(static_data::pl_23_a) == 55);
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spare3


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> spare4
template <> struct Result_Value<Field::spare4> { using type = std::uint32_t; };

template <>
constexpr auto
field_name<Field::spare4>() noexcept -> std::string_view {
  return "spare4";
}


template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
spare4(std::string_view s) noexcept -> aiscpp_result<value_T<Field::spare4>> {
  /*
   * `Msg_Class::m_15` is the only class with a 4th spare, but it isn't
   * guaranteed to be sufficiently long to actually have it... meaning the
   * templated decoding needs some help to prevent indexing past the end of
   * `s`'s underlying `char[]`.
   *
   *
   * From Eric S. Raymond's <esr@thyrsus.com>
   * "AIVDM/AIVDO protocol decoding"
   * <https://gpsd.gitlab.io/gpsd/AIVDM.html#_type_15_interrogation>:
   *
   * There are four use cases for this message. A decoder must dispatch on
   * the length of the data packet to determine which it is seeing:
   *
   * 1. One station is interrogated for one message type. Length is 88 bits.
   *
   * 2. One station is interrogated for two message types, Length is 110 bits.
   * There is a design error in the standard here; according to the <[ITU1371]>
   * requirement  for padding to 8 bits, this should have been 112 with a 4-bit
   * trailing spare  field, and decoders should be prepared to handle that
   * length as well. See the  discussion of byte alignment elsewhere in this
   * document for context.
   *
   * 3. Two stations are interrogated for one message type each. Length is 160
   * bits.  The second message type and second slot offset associated with the
   * first queried  MMSI should be zeroed.
   *
   * 4. One station is interrogated for two message types, and a second for one
   * message type. Length is 160 bits.
   *                         ^^^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   */ //                                                                     ^
  // intentionally verbose                                                   ^
  constexpr auto md       = meta::data<Msg_Class::m_15, Field::spare4>{}; // ^
  constexpr auto min_bits = md.first_bit + md.n_bits;                     // ^
  static_assert(min_bits == 160); // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>^
  constexpr auto min_chars = min_bits / 6;
  static_assert(min_chars == 26);
  // intentionally verbose

  if constexpr (msg_class == Msg_Class::unknown) {
    if (s[0] == '?') {
      if (std::size(s) < min_chars) {
        return status_code::NOT_AVAILABLE;
      } else {
        return decode_internal::decode<Msg_Class::m_15, Field::spare4>(s);
      }
    } else {
      return status_code::MSG_TYPE_HAS_NO_SUCH_FIELD;
    }
  } else if constexpr (msg_class == Msg_Class::m_15) {
    if (std::size(s) < min_chars) {
      return status_code::NOT_AVAILABLE;
    } else {
      return decode_internal::decode<msg_class, Field::spare4>(s);
    }
  } else {
    return decode_internal::decode<msg_class, Field::spare4>(s);
  }
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::spare4>() == "spare4");
// clang-format off
static_assert(spare4(static_data::pl_1_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_1_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_3_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_15_a).status() == status_code::NOT_AVAILABLE);
static_assert(spare4(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_18_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_19_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_27_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_27_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(spare4(static_data::pl_27_c).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);

static_assert(spare4(static_data::pl_15_a) == 0);
static_assert(spare4<Msg_Class::m_15>(static_data::pl_15_a) == 0);

static_assert(spare4<Msg_Class::m_15>(static_data::pl_15_a).status() == status_code::NOT_AVAILABLE);
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spare4


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> raim
template <> struct Result_Value<Field::raim> { using type = bool; };

template <>
constexpr auto
field_name<Field::raim>() noexcept -> std::string_view {
  return "raim";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
raim(std::string_view s) noexcept -> aiscpp_result<value_T<Field::raim>> {
  return decode_internal::decode<msg_class, Field::raim>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::raim>() == "raim");
// clang-format off
static_assert(raim(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(raim(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(raim(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(raim(static_data::pl_4_a).status() == status_code::SUCCESS);
static_assert(raim(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_9_a).status() == status_code::SUCCESS);
static_assert(raim(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_11_a).status() == status_code::SUCCESS);
static_assert(raim(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(raim(static_data::pl_19_a).status() == status_code::SUCCESS);
static_assert(raim(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_21_a).status() == status_code::SUCCESS);
static_assert(raim(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(raim(static_data::pl_27_a).status() == status_code::SUCCESS);
static_assert(raim(static_data::pl_27_b).status() == status_code::SUCCESS);
static_assert(raim(static_data::pl_27_c).status() == status_code::SUCCESS);

static_assert(!raim(static_data::pl_1_a));
static_assert(!raim(static_data::pl_1_b));
static_assert(!raim(static_data::pl_3_a));
static_assert(!raim(static_data::pl_4_a));
static_assert(!raim(static_data::pl_9_a));
static_assert(!raim(static_data::pl_11_a));
static_assert(raim(static_data::pl_18_a));
static_assert(!raim(static_data::pl_19_a));
static_assert(!raim(static_data::pl_21_a));
static_assert(!raim(static_data::pl_27_a));
static_assert(!raim(static_data::pl_27_b));
static_assert(!raim(static_data::pl_27_c));

static_assert(!raim<Msg_Class::m_1_2_3>(static_data::pl_1_a));
static_assert(!raim<Msg_Class::m_1_2_3>(static_data::pl_1_b));
static_assert(!raim<Msg_Class::m_1_2_3>(static_data::pl_3_a));
static_assert(!raim<Msg_Class::m_4_11>(static_data::pl_4_a));
static_assert(!raim<Msg_Class::m_9>(static_data::pl_9_a));
static_assert(!raim<Msg_Class::m_4_11>(static_data::pl_11_a));
static_assert(raim<Msg_Class::m_18>(static_data::pl_18_a));
static_assert(!raim<Msg_Class::m_19>(static_data::pl_19_a));
static_assert(!raim<Msg_Class::m_21>(static_data::pl_21_a));
static_assert(!raim<Msg_Class::m_27>(static_data::pl_27_a));
static_assert(!raim<Msg_Class::m_27>(static_data::pl_27_b));
static_assert(!raim<Msg_Class::m_27>(static_data::pl_27_c));
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< raim


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> sync_state
template <> struct Result_Value<Field::sync_state> {
  using type = std::uint32_t;
};

template <>
constexpr auto
field_name<Field::sync_state>() noexcept -> std::string_view {
  return "sync_state";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
sync_state(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::sync_state>> {
  return decode_internal::decode<msg_class, Field::sync_state>(s);
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::sync_state>() == "sync_state");
// clang-format off
static_assert(sync_state(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(sync_state(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(sync_state(static_data::pl_3_a).status() == status_code::SUCCESS);
static_assert(sync_state(static_data::pl_4_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_11_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(sync_state(static_data::pl_19_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_27_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_27_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(sync_state(static_data::pl_27_c).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);

static_assert(sync_state(static_data::pl_1_a) == 1);
static_assert(sync_state(static_data::pl_1_b) == 0);
static_assert(sync_state(static_data::pl_3_a) == 0);
static_assert(sync_state(static_data::pl_18_a) == 3);

static_assert(sync_state<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 1);
static_assert(sync_state<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 0);
static_assert(sync_state<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 0);
static_assert(sync_state<Msg_Class::m_18>(static_data::pl_18_a) == 3);
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< sync_state


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> slot_timeout
template <> struct Result_Value<Field::slot_timeout> {
  using type = std::uint32_t;
};

template <>
constexpr auto
field_name<Field::slot_timeout>() noexcept -> std::string_view {
  return "slot_timeout";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
slot_timeout(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::slot_timeout>> {
  if constexpr (msg_class == Msg_Class::unknown) {
    switch (get_msg_id(s)) {
        // message type 3 doesn't have it, but the rest of m_1_2_3 do, so
        // skipping `decode_field()`'s naive branch...
      case Msg_ID::i_1:
        [[fallthrough]];
      case Msg_ID::i_2:
        return slot_timeout<Msg_Class::m_1_2_3>(s);
      case Msg_ID::i_3:
        return status_code::MSG_TYPE_HAS_NO_SUCH_FIELD;
      case Msg_ID::i_4:
        [[fallthrough]];
      case Msg_ID::i_11:
        return slot_timeout<Msg_Class::m_4_11>(s);
      case Msg_ID::i_18:
        return slot_timeout<Msg_Class::m_18>(s);
      default:
        return status_code::MSG_TYPE_HAS_NO_SUCH_FIELD;
    }
  } else if constexpr (msg_class == Msg_Class::m_1_2_3) {
    return get_msg_id(s) != Msg_ID::i_3
               ? decode_internal::decode<msg_class, Field::slot_timeout>(s)
               : status_code::MSG_TYPE_HAS_NO_SUCH_FIELD;
  } else {
    return decode_internal::decode<msg_class, Field::slot_timeout>(s);
  }
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::slot_timeout>() == "slot_timeout");
// clang-format off
static_assert(slot_timeout(static_data::pl_1_a).status() == status_code::SUCCESS);
static_assert(slot_timeout(static_data::pl_1_b).status() == status_code::SUCCESS);
static_assert(slot_timeout(static_data::pl_3_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_4_a).status() == status_code::SUCCESS);
static_assert(slot_timeout(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_11_a).status() == status_code::SUCCESS);
static_assert(slot_timeout(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(slot_timeout(static_data::pl_19_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_27_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_27_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_timeout(static_data::pl_27_c).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);

static_assert(slot_timeout(static_data::pl_1_a) == 1);
static_assert(slot_timeout(static_data::pl_1_b) == 6);
static_assert(slot_timeout(static_data::pl_4_a) == 2);
static_assert(slot_timeout(static_data::pl_11_a) == 0);
static_assert(slot_timeout(static_data::pl_18_a) == 0);

static_assert(slot_timeout<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 1);
static_assert(slot_timeout<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 6);
static_assert(slot_timeout<Msg_Class::m_4_11>(static_data::pl_4_a) == 2);
static_assert(slot_timeout<Msg_Class::m_4_11>(static_data::pl_11_a) == 0);
static_assert(slot_timeout<Msg_Class::m_18>(static_data::pl_18_a) == 0);
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< slot_timeout


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> slot_offset
template <> struct Result_Value<Field::slot_offset> {
  using type = std::uint32_t;
};

template <>
constexpr auto
field_name<Field::slot_offset>() noexcept -> std::string_view {
  return "slot_offset";
}

template <Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
slot_offset(std::string_view s) noexcept
    -> aiscpp_result<value_T<Field::slot_offset>> {
  if constexpr (msg_class != Msg_Class::unknown) {
    if (const auto [so, status] =
            decode_internal::decode<msg_class, Field::slot_offset>(s);
        status == status_code::SUCCESS && so == 0) {
      return decode_internal::decode<msg_class, Field::slot_offset>(s);
    }
    return status_code::NOT_AVAILABLE;

  } else { // msg_class == Msg_Class::unknown
    switch (get_msg_id(s)) {
      case Msg_ID::i_1:
        [[fallthrough]];
      case Msg_ID::i_2:
        [[fallthrough]];
      case Msg_ID::i_3:
        [[fallthrough]];
        return slot_offset<Msg_Class::m_1_2_3>(s);
      case Msg_ID::i_4:
        [[fallthrough]];
      case Msg_ID::i_11:
        return slot_offset<Msg_Class::m_4_11>(s);
      case Msg_ID::i_18:
        return slot_offset<Msg_Class::m_18>(s);
      default:
        return status_code::MSG_TYPE_HAS_NO_SUCH_FIELD;
    }
  }
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
static_assert(field_name<Field::slot_offset>() == "slot_offset");
// clang-format off
static_assert(slot_offset(static_data::pl_1_a).status() == status_code::NOT_AVAILABLE);
static_assert(slot_offset(static_data::pl_1_b).status() == status_code::NOT_AVAILABLE);
static_assert(slot_offset(static_data::pl_3_a).status() == status_code::NOT_AVAILABLE);
static_assert(slot_offset(static_data::pl_4_a).status() == status_code::NOT_AVAILABLE);
static_assert(slot_offset(static_data::pl_5_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_6_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_7_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_8_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_9_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_10_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_11_a).status() == status_code::SUCCESS);
static_assert(slot_offset(static_data::pl_12_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_13_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_14_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_15_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_16_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_17_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_18_a).status() == status_code::SUCCESS);
static_assert(slot_offset(static_data::pl_19_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_20_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_21_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_22_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_23_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_24_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_25_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_26_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_27_a).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_27_b).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);
static_assert(slot_offset(static_data::pl_27_c).status() == status_code::MSG_TYPE_HAS_NO_SUCH_FIELD);

static_assert(slot_offset(static_data::pl_1_a) == 0);
static_assert(slot_offset(static_data::pl_1_b) == 0);
static_assert(slot_offset(static_data::pl_3_a) == 0);
static_assert(slot_offset(static_data::pl_4_a) == 0);
static_assert(slot_offset(static_data::pl_11_a) == 0);
static_assert(slot_offset(static_data::pl_18_a) == 0);

static_assert(slot_offset<Msg_Class::m_1_2_3>(static_data::pl_1_a) == 0);
static_assert(slot_offset<Msg_Class::m_1_2_3>(static_data::pl_1_b) == 0);
static_assert(slot_offset<Msg_Class::m_1_2_3>(static_data::pl_3_a) == 0);
static_assert(slot_offset<Msg_Class::m_4_11>(static_data::pl_4_a) == 0);
static_assert(slot_offset<Msg_Class::m_4_11>(static_data::pl_11_a) == 0);
static_assert(slot_offset<Msg_Class::m_18>(static_data::pl_18_a) == 0);
// clang-format on
} // namespace static_tests
#endif
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< slot_offset


// =============================================================================
// =============================================================================
// =============================================================================
// =============================================================================
// =============================================================================
// =============================================================================
// =============================================================================
template <Field field, Msg_Class msg_class = Msg_Class::unknown>
constexpr auto
decode_field(std::string_view s) noexcept -> aiscpp_result<value_T<field>> {
  if constexpr (field == Field::msg_id) {
    return msg_id<msg_class>(s);
  }
  if constexpr (field == Field::repeat_indicator) {
    return repeat_indicator<msg_class>(s);
  }
  if constexpr (field == Field::mmsi) {
    return mmsi<msg_class>(s);
  }
  if constexpr (field == Field::nav_status) {
    return nav_status<msg_class>(s);
  }
  if constexpr (field == Field::rate_of_turn) {
    return rate_of_turn<msg_class>(s);
  }
  if constexpr (field == Field::speed_over_ground) {
    return speed_over_ground<msg_class>(s);
  }
  if constexpr (field == Field::position_accuracy) {
    return position_accuracy<msg_class>(s);
  }
  if constexpr (field == Field::longitude) {
    return longitude<msg_class>(s);
  }
  if constexpr (field == Field::latitude) {
    return latitude<msg_class>(s);
  }
  if constexpr (field == Field::course_over_ground) {
    return course_over_ground<msg_class>(s);
  }
  if constexpr (field == Field::true_heading) {
    return true_heading<msg_class>(s);
  }
  if constexpr (field == Field::timestamp) {
    return timestamp<msg_class>(s);
  }
  if constexpr (field == Field::special_maneuver) {
    return special_maneuver<msg_class>(s);
  }
  if constexpr (field == Field::spare) {
    return spare<msg_class>(s);
  }
  if constexpr (field == Field::spare2) {
    return spare2<msg_class>(s);
  }
  if constexpr (field == Field::spare3) {
    return spare3<msg_class>(s);
  }
  if constexpr (field == Field::spare4) {
    return spare4<msg_class>(s);
  }
  if constexpr (field == Field::raim) {
    return raim<msg_class>(s);
  }
  if constexpr (field == Field::sync_state) {
    return sync_state<msg_class>(s);
  }
  if constexpr (field == Field::slot_timeout) {
    return slot_timeout<msg_class>(s);
  }
  if constexpr (field == Field::slot_offset) {
    return slot_offset<msg_class>(s);
  }
}
#if AISCPP_ENABLE_STATIC_TESTS
namespace static_tests {
// clang-format off
static_assert(decode_field<Field::msg_id>(static_data::pl_1_a) == 1);
static_assert(decode_field<Field::msg_id, Msg_Class::m_1_2_3>(static_data::pl_1_a) == 1);
// clang-format on
} // namespace static_tests
#endif

} // namespace aiscpp


#endif
