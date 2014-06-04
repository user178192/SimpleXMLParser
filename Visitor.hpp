#ifndef YH_VISITOR
#define YH_VISITOR
namespace xml {

    class Visitor {
    public:
        virtual void start_element_visit(const Element &) = 0;
        virtual void end_element_visit(const Element &) = 0;
        virtual void visit_text(const Text &) = 0;
    };
}

#endif
