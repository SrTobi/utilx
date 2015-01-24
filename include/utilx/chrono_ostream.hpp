#pragma once
#ifndef _UTILX_ADV_OSTREAM_HPP
#define _UTILX_ADV_OSTREAM_HPP


#include <chrono>
#include <ostream>


namespace std {
	namespace chrono {

		template<typename Ch, typename Rep>
		std::basic_ostream<Ch>& operator << (std::basic_ostream<Ch>& _stream, const std::chrono::duration<Rep, std::ratio<1,1>>& _dur)
		{
			_stream << _dur.count() << "sec";
			return _stream;
		}

		template<typename Ch, typename Rep>
		std::basic_ostream<Ch>& operator << (std::basic_ostream<Ch>& _stream, const std::chrono::duration<Rep, std::milli>& _dur)
		{
			_stream << _dur.count() << "ms";
			return _stream;
		}

		template<typename Ch, typename Rep, typename Ratio>
		std::basic_ostream<Ch>& operator << (std::basic_ostream<Ch>& _stream, const std::chrono::duration<Rep, Ratio>& _dur)
		{
			_stream << std::chrono::duration_cast<std::chrono::nanoseconds>(_dur.count()) << "ns";
			return _stream;
		}





	}
}



#endif