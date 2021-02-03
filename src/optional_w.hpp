#ifndef OPTIONAL_W_HPP
#define OPTIONAL_W_HPP

#include <algorithm>

template <typename Enum_t, Enum_t default_value = Enum_t{} >
class info
{
public:
  using E = Enum_t;

  constexpr info() : mE{} {}

  constexpr info(const Enum_t &v) : mE{v} {}

  constexpr operator bool() const { return mE != default_value; }
  constexpr const Enum_t &status() const { return mE; }

private:
  Enum_t mE;
};

template <typename Enum_t, Enum_t default_value = Enum_t{}, std::size_t Msg_sz = 16 >
class info_descr : public info<Enum_t>
{
public:
  using info<Enum_t>::info;
  using E = typename info<Enum_t>::E;

  constexpr info_descr(const info<Enum_t> &i)
    : info<Enum_t>{i}
  {
  }

  template <std::size_t N>
  constexpr info_descr(const Enum_t &v, const char(&desc)[N])
    : info_descr{v, desc, std::make_index_sequence<std::min(N, Msg_sz - 1)>()}
  {
  }

  template <std::size_t N>
  constexpr info_descr &set_description(const char(&s)[N])
  {
    std::copy(s, s + std::min(Msg_sz - 1, N), mM);
    return *this;
  }

  constexpr const char *description() { return mM; }

private:
  template <std::size_t N, std::size_t... idx>
  constexpr info_descr(const Enum_t &v, const char(&desc)[N], std::index_sequence<idx...>)
    : info<Enum_t>{v}, mM{desc[idx]...}
  {
  }

  char mM[Msg_sz]{};
};

template <typename El_t, typename Enum_t, Enum_t set_value, Enum_t default_value = Enum_t{} >
class optional_winfo
{
public:
  using E = Enum_t;

  constexpr optional_winfo() : mE{}, mZ{} {}

  constexpr optional_winfo(const El_t &val) : mE{set_value}, mT{val} {}

  constexpr optional_winfo(const Enum_t &s) : mE{s}, mZ{} {}

  ~optional_winfo()
  {
    if (mE == set_value)
      mT.~El_t();
  }

  constexpr operator bool() const { return mE != default_value; }
  constexpr Enum_t status() const { return mE; }

  constexpr bool is_set() const { return mE == set_value; }

  constexpr El_t &value() { return mT; }
  constexpr El_t value_or(const El_t &other) const { return is_set() ? mT : other; }

  constexpr optional_winfo &operator=(const El_t &value)
  {
    if (mE == set_value)
      mT.~El_t();
    mT = value;
    mE = set_value;
    return *this;
  }
  constexpr optional_winfo &operator=(const Enum_t &s)
  {
    if ((mE == set_value) && (s != set_value))
      mT.~El_t();
    mE = s;
    return *this;
  }

private:
  Enum_t mE;
  union
  {
    El_t mT;
    char mZ[sizeof(El_t)];
  };
};

template <typename El_t, typename Enum_t, Enum_t set_value, Enum_t default_value = Enum_t{}, std::size_t Msg_sz = 16 >
class optional_wdesc : public optional_winfo<El_t, Enum_t, set_value, default_value>
{
public:
  using optional_winfo<El_t, Enum_t, set_value, default_value>::optional_winfo;
  using E = typename optional_winfo<El_t, Enum_t, set_value, default_value>::E;

  constexpr optional_wdesc(const optional_winfo<El_t, Enum_t, set_value, default_value> &oi)
    : optional_winfo<El_t, Enum_t, set_value, default_value>{oi}
  {
  }

  template <std::size_t N>
  constexpr optional_wdesc(const El_t &val, const char(&desc)[N])
    : optional_wdesc{val, desc, std::make_index_sequence<std::min(N, Msg_sz - 1)>()}
  {
  }

  template <std::size_t N>
  constexpr optional_wdesc(const Enum_t &s, const char(&desc)[N])
    : optional_wdesc{s, desc, std::make_index_sequence<std::min(N, Msg_sz - 1)>()}
  {
  }

  template <std::size_t N>
  constexpr optional_wdesc &set_description(const char(&s)[N])
  {
    std::copy(s, s + std::min(Msg_sz - 1, N), mM);
    return *this;
  }

  constexpr const char *description() { return mM; }

private:
  template <std::size_t N, std::size_t... idx>
  constexpr optional_wdesc(const El_t &val, const char(&desc)[N], std::index_sequence<idx...>)
    : optional_winfo<El_t, Enum_t, set_value, default_value>{val}, mM{desc[idx]...}
  {
  }

  template <std::size_t N, std::size_t... idx>
  constexpr optional_wdesc(const Enum_t &s, const char(&desc)[N], std::index_sequence<idx...>)
    : optional_winfo<El_t, Enum_t, set_value, default_value>{s}, mM{desc[idx]...}
  {
  }

  char mM[Msg_sz];
};

#endif // OPTIONAL_W_HPP
