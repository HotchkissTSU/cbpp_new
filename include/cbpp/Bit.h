#ifndef CBPP_BITWORK_H
#define CBPP_BITWORK_H

#include <stdint.h>
#include <stdio.h>

namespace cbpp {
    bool HasZeroBits(uint64_t iNum);

	template <typename INT_T> bool TestBitmask(INT_T word, INT_T bmask){
		return (word & bmask) == bmask;
	}
	
	template <typename INT_T> uint8_t GetBit(INT_T word, uint16_t index){
		return (word & ((INT_T)(1) << index)) >> index;
	}
	
	template <typename INT_T> void SetBit(INT_T& word, uint16_t index, uint8_t bit){
        word = bit ? (word | ((INT_T)(1) << index)) : (word & ~((INT_T)(1) << index));
	}

	template <typename INT_T> void PrintBits(INT_T word) {
		for(uint8_t i = 0; i < sizeof(word)*8; i++) {
			if(i % 8 == 0) {
				putc(' ', stdout);
			}
			printf("%u", GetBit<INT_T>(word, 8-i-1) ? 1 : 0 );
		}
	}
}

#endif
