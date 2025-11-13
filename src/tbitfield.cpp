// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw "Length must be positive!";
	BitLen = len;
	MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField() // деструктор
{
	delete[] pMem;
	pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen))
		throw "Bit out of range";
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] = pMem[index] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen))
		throw "Bit out of range";
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] = pMem[index] & (~mask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen))
		throw "Bit out of range";
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	return (pMem[index] & mask) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return *this;
	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int maxBitLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	TBitField res(maxBitLen);
	int minMemLen = (MemLen < res.MemLen) ? MemLen : res.MemLen;
	for (int i = 0; i < minMemLen; i++)
		res.pMem[i] = pMem[i];
	minMemLen = (bf.MemLen < res.MemLen) ? bf.MemLen : res.MemLen;
	for (int i = 0; i < minMemLen; i++)
		res.pMem[i] = res.pMem[i] | bf.pMem[i];
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int maxBitLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	TBitField res(maxBitLen);
	int minBitLen = (BitLen < bf.BitLen) ? BitLen : bf.BitLen;
	int minMemLen = (MemLen < res.MemLen) ? MemLen : res.MemLen;
	for (int i = 0; i < minMemLen; i++)
		res.pMem[i] = pMem[i];
	minMemLen = (bf.MemLen < res.MemLen) ? bf.MemLen : res.MemLen;
	for (int i = 0; i < minMemLen; i++)
		res.pMem[i] = res.pMem[i] & bf.pMem[i];
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < MemLen; i++)
		res.pMem[i] = ~pMem[i];
	if ((BitLen % (sizeof(TELEM) * 8)) != 0) {
        TELEM mask = (1 << BitLen % (sizeof(TELEM) * 8)) - 1;
        res.pMem[MemLen - 1] &= mask;
    }
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	for (int i = 0; i < bf.BitLen; i++) {
		char bit;
		istr >> bit;
		if (bit == '1') bf.SetBit(i);
		else if (bit == '0') bf.ClrBit(i);
		else throw "Wrong bit!";
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << (bf.GetBit(i) ? '1': '0');
		if (((i + 1) % 8 == 0) && (i!= bf.BitLen -1))
			ostr << ' ';
	}
	return ostr;
}
