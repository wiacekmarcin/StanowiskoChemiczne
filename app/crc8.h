#ifndef CRC8_H
#define CRC8_H

#define CRC8_DEFAULT_POLYNOME       0x07

#include <stdint.h>
/**
 * @brief The CRC8 class
 * Klasa do weryfikacji protokołu
 */
class CRC8
{
public:
    /**
   * @brief CRC8 Konstruktor
   */
  CRC8();

  // set parameters to default
  void     reset();       // set all to constructor defaults
  void     restart();     // reset crc with same parameters.

  // set parameters
  void     setPolynome(uint8_t polynome)  { _polynome = polynome; };
  void     setStartXOR(uint8_t start)     { _startMask = start; };
  void     setEndXOR(uint8_t end)         { _endMask = end; };
  void     setReverseIn(bool reverseIn)   { _reverseIn = reverseIn; };
  void     setReverseOut(bool reverseOut) { _reverseOut = reverseOut; };

  /**
   * @brief add dodanie do crc kolejnego znakuu
   * @param value - znak
   */
  void     add(uint8_t value);

  /**
   * @brief add dodanie kolejnych znakow do liczenia crc
   * @param array tablica kolejnych znaków
   * @param length długość tablicy
   */
  void     add(uint8_t * array, uint32_t length);

  /**
   * @brief getCRC pobiera znak crc
   * @return
   */
  uint8_t  getCRC();  // returns CRC


  uint32_t count()    { return _count; };

private:
  uint8_t  _reverse(uint8_t value);
  void     _update(uint8_t value);

  uint8_t  _polynome;
  uint8_t  _startMask;
  uint8_t  _endMask;
  uint8_t  _crc;
  bool     _reverseIn;
  bool     _reverseOut;
  bool     _started;
  uint32_t _count;
};

#endif // CRC8_H
