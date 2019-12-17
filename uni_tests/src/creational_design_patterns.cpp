#include "creational_design_patterns.h"
#include "aps/templates/factory.h"
#include "aps/templates/singleton.h"

class Animal_t {
public:
    typedef enum animal_sounds {
        HAV = 0,
        MEOW,
        NONE
    } animal_sounds;
    virtual ~Animal_t(){}
    virtual animal_sounds Sound(){return NONE;}
private:
};



class Dog_t: public Animal_t {
public:
    ~Dog_t(){}
    Dog_t(){}
    animal_sounds Sound(){return HAV;}
private:
};


class Cat_t:public Animal_t {
public:
    ~Cat_t(){}
    Cat_t(){}
    animal_sounds Sound(){return MEOW;}
private:
};


Animal_t* newDOG() {
    return new Dog_t;
}

cdp_test_result abstract_factory_create_object() {
    alexei_prog_snob::abstract_factory<Animal_t> animals;
    REG(animals, Dog_t, Animal_t);

    std::shared_ptr<Animal_t> newOne = animals.make_object("Dog_t");
    if (newOne == nullptr) {
        return CDP_TEST_FAIL;
    }

    return CDP_TEST_PASS;
}

cdp_test_result abstract_factory_check_constructed_correct_obj() {
    alexei_prog_snob::abstract_factory<Animal_t> animals;
    REG(animals, Dog_t, Animal_t);
    REG(animals, Animal_t, Animal_t);

    std::shared_ptr<Animal_t> newOne = animals.make_object("Dog_t");
    if (newOne == nullptr) {
        return CDP_TEST_FAIL;
    }

    if (newOne->Sound() != Animal_t::HAV) {
        return CDP_TEST_FAIL;
    }

    std::shared_ptr<Animal_t> newTwo = animals.make_object("Animal_t");
    if (newTwo == nullptr) {
        return CDP_TEST_FAIL;
    }

    if (newTwo->Sound() != Animal_t::NONE) {
        return CDP_TEST_FAIL;
    }

    return CDP_TEST_PASS;
}


class Counter {
public:
    template<class T>
    friend class alexei_prog_snob::singleton;

    Counter& operator= (const Counter&) = delete;
    Counter(const Counter&) = delete;

private:
    Counter():m_count(0){}
    ~Counter(){}

    size_t m_count;
};

cdp_test_result singleton_check_that_only_one_instance_of_the_class() {
    Counter* c1 = alexei_prog_snob::singleton<Counter>::get_instance();
    Counter* c2 = alexei_prog_snob::singleton<Counter>::get_instance();
    if (c1 != c2) {
        return CDP_TEST_FAIL;
    }

    return CDP_TEST_PASS;
}
