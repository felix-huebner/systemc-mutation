#ifndef FISIGNAL_H_
#define FISIGNAL_H_

#include <string>
#include <limits>

#include <systemc.h>

template<typename T>
class fi_signal : public sc_buffer<T> {
public:
	typedef fi_signal<T> this_type;

	fi_signal() :
		m_mutated_bit(std::numeric_limits<unsigned int>::max()),
		m_mutated_bit_value(false)
		{

	}

	virtual ~fi_signal() {

	}


    this_type& operator = ( const T& a )
	{ this->write( a ); return *this; }

    void set_mutation(unsigned int bit, bool val) {
    	m_mutated_bit=bit;
    	m_mutated_bit_value=val;
    }

    virtual const T& read() const {
    	if(m_mutated_bit==std::numeric_limits<unsigned int>::max()) {
    		return sc_signal<T>::read();
    	}
    	m_result=sc_signal<T>::read();
    	unsigned char* raw_mem=reinterpret_cast<unsigned char*>(&m_result);
    	unsigned char* byte_to_mutate=&raw_mem[m_mutated_bit/8];

    	if(m_mutated_bit_value) {
    		//set bit
    		(*byte_to_mutate) |= (1<<(m_mutated_bit%8));
    	} else {
    		//clear bit
    		(*byte_to_mutate) &= ~(1<<(m_mutated_bit%8));
    	}
    	return m_result;
    }
private:
	unsigned int m_mutated_bit;
	bool m_mutated_bit_value;
	mutable T m_result;
};


//unnoetig, allgemeine form funktioniert auch fuer bool, standard-konform/undefined behaviour?
//template<>
//class fi_signal<bool> : public sc_signal<bool> {
//public:
//	typedef fi_signal<bool> this_type;
//
//	fi_signal() :
//		m_mutated_bit(std::numeric_limits<unsigned int>::max()),
//		m_mutated_bit_value(false),
//		m_result(false)
//		{
//
//	}
//
//	virtual ~fi_signal() {
//
//	}
//
//
//    this_type& operator = ( const bool& a )
//	{ this->write( a ); return *this; }
//
//    void set_mutation(unsigned int bit, bool val) {
//    	m_mutated_bit=bit;
//    	m_mutated_bit_value=val;
//    }
//
//    virtual const bool& read() const {
//    	if(m_mutated_bit==std::numeric_limits<unsigned int>::max()) {
//    		return sc_signal<bool>::read();
//    	}
//    	m_result=m_mutated_bit_value;
//
//    	return m_result;
//    }
//private:
//	unsigned int m_mutated_bit;
//	bool m_mutated_bit_value;
//	mutable bool m_result;
//};


#endif /* FISIGNAL_H_ */
