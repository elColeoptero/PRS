//
// Created by elcoleoptero on 30/03/2021.
//

#ifndef CHES_BITBOARD_H
#define CHES_BITBOARD_H
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int msb(uint64_t b);
uint64_t byteswap(uint64_t b);
uint64_t king_attacks(int sq);
uint64_t calc_king_attacks(uint64_t kings);
uint64_t knight_attacks(int sq) ;
uint64_t calc_knight_attacks(uint64_t knights);
uint64_t pawn_attacks(unsigned cl, int sq);
//template <unsigned Cl>
uint64_t calc_pawn_attacks(uint64_t pawns,int Cl);
uint64_t rank_attacks(int sq, uint64_t occ);
uint64_t file_attacks(int sq, uint64_t occ);
uint64_t calc_rook_attacks(int sq, uint64_t occ);
// Use Hyperbola Quintessence for bishop attack generation
uint64_t diag_attacks(int sq, uint64_t occ);
uint64_t adiag_attacks(int sq, uint64_t occ);
uint64_t calc_bishop_attacks(int sq, uint64_t occ);
uint64_t rook_attacks(int sq, uint64_t occ) ;
uint64_t bishop_attacks(int sq, uint64_t occ) ;
uint64_t queen_attacks(int sq, uint64_t occ);
uint64_t next_subset(const uint64_t Set, const uint64_t subset);
void init();
void init_piece(bool rook, int sq);
void printSize();
#endif //CHES_BITBOARD_H
