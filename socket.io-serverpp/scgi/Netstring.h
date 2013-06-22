#pragma once

#include <string>
#include <boost/lexical_cast.hpp>
#include <map>

namespace utils_private
{
using std::map;
using std::string;

template <class T>
class Netstring 
{
    public:
        typedef map<string, string> value_t;

        Netstring(const T& input) 
        :m_input(input)
        ,m_p(input.begin())
        {
            readSize();

            auto endstr = m_p + m_size;

            while (m_p < endstr && m_p != 0 ) {
                string tag(m_p);
                m_p += tag.size() + 1;
                string value(m_p);
                m_p += value.size() + 1;

                m_values[tag] = value;
            }
        }

        operator value_t()
        {
            return m_values;
        }

    private:
        void readSize() {
            auto doublecolonptr = std::find(m_p, m_input.end(), ':');
            string sizeStr(m_p, doublecolonptr);
            m_size = boost::lexical_cast<size_t>(sizeStr);
            m_p = doublecolonptr + 1;
        }

        map<string, string> m_values;
        const T& m_input;
        typename T::const_iterator m_p;
        size_t m_size;
};
}

namespace utils {
    using utils_private::Netstring;

    template <class T>
    std::map<std::string, std::string> netstring2map(const T& input) {
        return Netstring<T>(input);
    }
}
