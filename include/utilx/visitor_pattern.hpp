#pragma once
#ifndef _VISITOR_PATTERN_HPP
#define _VISITOR_PATTERN_HPP

namespace utilx {


	template<typename Visitor>
	class visitable
	{
	public:
		typedef Visitor visitor_type;
		virtual ~visitable() = default;

		virtual void accept(Visitor&) = 0;
	};

#define UTILX_DEFINE_ACCEPT(Visitor) \
	virtual void accept(Visitor& v) override \
	{ \
		v.visit(*this); \
	}

#define UTILX_EXTEND_VISITABLE(BaseVisitor, SubVisitor) \
	virtual void accept(BaseVisitor& bv) override \
	{ \
		accept(dynamic_cast<SubVisitor&>(bv)); \
	}


}


#endif