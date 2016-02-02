#include "testx/testx.hpp"
#include "utilx/visitor_pattern.hpp"


namespace visitor_pattern_tests {

	struct Base;
	struct Sub1;
	struct Sub2;

	struct Visitor
	{
		virtual void visit(Sub1&) = 0;
		virtual void visit(Sub2&) = 0;
		virtual void visit(Base&) = 0;
	};

	struct Base: public utilx::visitable<Visitor>
	{
		virtual void accept(Visitor& v) override
		{
			v.visit(*this);
		}
	};

	struct Sub1 : public Base
	{
		virtual void accept(Visitor& v) override
		{
			v.visit(*this);
		}
	};

	struct Sub2 : public Base
	{
		virtual void accept(Visitor& v) override
		{
			v.visit(*this);
		}
	};


	TESTX_AUTO_TEST_CASE(_base_visitor_test)
	{
		
		struct BaseTestVisitor
			: public Visitor
		{
			virtual void visit(Sub1&) override
			{
				sub1Visited = true;
			}

			virtual void visit(Sub2&) override
			{
				sub2Visited = true;
			}

			virtual void visit(Base&) override
			{
				baseVisited = true;
			}

			bool baseVisited = false;
			bool sub1Visited = false;
			bool sub2Visited = false;
		};

		// Test base
		{
			BaseTestVisitor v;
			Base b;
			Base& ref = b;
			ref.accept(v);
			BOOST_CHECK_EQUAL(v.baseVisited, true);
			BOOST_CHECK_EQUAL(v.sub1Visited, false);
			BOOST_CHECK_EQUAL(v.sub2Visited, false);
		}

		// Test sub1
		{
			BaseTestVisitor v;
			Sub1 s;
			Base& ref = s;
			ref.accept(v);
			BOOST_CHECK_EQUAL(v.baseVisited, false);
			BOOST_CHECK_EQUAL(v.sub1Visited, true);
			BOOST_CHECK_EQUAL(v.sub2Visited, false);
		}

		// Test sub2
		{
			BaseTestVisitor v;
			Sub2 s;
			Base& ref = s;
			ref.accept(v);
			BOOST_CHECK_EQUAL(v.baseVisited, false);
			BOOST_CHECK_EQUAL(v.sub1Visited, false);
			BOOST_CHECK_EQUAL(v.sub2Visited, true);
		}
	}



	struct ExtendedSub;

	struct ExtendedVisitor : public Visitor
	{
		virtual void visit(ExtendedSub&) = 0;
	};

	struct ExtendedSub
		: public Sub1
		, public utilx::visitable<ExtendedVisitor>
	{
		UTILX_EXTEND_VISITABLE(Visitor, ExtendedVisitor);

		virtual void accept(ExtendedVisitor& v) override
		{
			v.visit(*this);
		}
	};

	TESTX_AUTO_TEST_CASE(_extended_visitor_test)
	{

		struct ExtendedTestVisitor
			: public ExtendedVisitor
		{
			virtual void visit(Sub1&) override
			{
				sub1Visited = true;
			}

			virtual void visit(Sub2&) override
			{
				sub2Visited = true;
			}

			virtual void visit(Base&) override
			{
				baseVisited = true;
			}

			virtual void visit(ExtendedSub&) override
			{
				exSubVisited = true;
			}

			bool baseVisited = false;
			bool sub1Visited = false;
			bool sub2Visited = false;
			bool exSubVisited = false;
		};

		// Test base
		{
			ExtendedTestVisitor v;
			Base b;
			Base& ref = b;
			ref.accept(v);
			BOOST_CHECK_EQUAL(v.baseVisited, true);
			BOOST_CHECK_EQUAL(v.sub1Visited, false);
			BOOST_CHECK_EQUAL(v.sub2Visited, false);
			BOOST_CHECK_EQUAL(v.exSubVisited, false);
		}

		// Test sub1
		{
			ExtendedTestVisitor v;
			Sub1 s;
			Base& ref = s;
			ref.accept(v);
			BOOST_CHECK_EQUAL(v.baseVisited, false);
			BOOST_CHECK_EQUAL(v.sub1Visited, true);
			BOOST_CHECK_EQUAL(v.sub2Visited, false);
			BOOST_CHECK_EQUAL(v.exSubVisited, false);
		}

		// Test sub2
		{
			ExtendedTestVisitor v;
			Sub2 s;
			Base& ref = s;
			ref.accept(v);
			BOOST_CHECK_EQUAL(v.baseVisited, false);
			BOOST_CHECK_EQUAL(v.sub1Visited, false);
			BOOST_CHECK_EQUAL(v.sub2Visited, true);
			BOOST_CHECK_EQUAL(v.exSubVisited, false);
		}

		// Test exSub
		{
			ExtendedTestVisitor v;
			ExtendedSub s;
			Base& ref = s;
			ref.accept(v);
			BOOST_CHECK_EQUAL(v.baseVisited, false);
			BOOST_CHECK_EQUAL(v.sub1Visited, false);
			BOOST_CHECK_EQUAL(v.sub2Visited, false);
			BOOST_CHECK_EQUAL(v.exSubVisited, true);
		}
	}

}