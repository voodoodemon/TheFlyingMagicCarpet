/* NightriderShow.h
 *
 * A basic version of the Nightrider light effect, which is just a simple chase that
 * starts in the corners and bounces off the middles. Lame, but it lights up the
 * carpet and doesn't rely on the sound.
 *
 * Author: Anders Linn
 * Date: August 2017
 */

#include "LightShow.h"

class NightriderShow : public LightShow {
 public:
   NightriderShow( MagicCarpet * carpetArg ) : LightShow( carpetArg ) {}

   void start() {
      for ( int i = NEO3_OFFSET; i < NEO4_OFFSET; ++i ) {
         carpet->ropeLeds[ i ] = CRGB::Black;
      }
   }

   void update( uint32_t time ) {
      static const CRGBPalette256 clr( CRGB::Red, CRGB::Black );
      static uint32_t timestamp = time;
      static const uint32_t rate = 300; // move the lights every 200ms
      static const uint32_t littleRate = rate;
      static const uint32_t bigRate = ( littleRate * SIZEOF_LARGE_NEO ) / SIZEOF_SMALL_NEO;
      static uint8_t bigPos = 0;
      static uint8_t littlePos = 0;
      static uint8_t bigPosDir = 1;
      static uint8_t littlePosDir = 1;

      uint32_t diff = time - timestamp;
      if ( true || diff > rate ) {
         timestamp = time;
         diff = 0;
         if ( bigPos == ( SIZEOF_LARGE_NEO * 2 ) - 1 ) {
            bigPosDir = 0;
         } else if ( bigPos == 0 && bigPosDir == 0 ) {
            bigPosDir = 1;
         }
         if ( littlePos == SIZEOF_SMALL_NEO - 1 ) {
            littlePosDir = 0;
         } else if ( littlePos == 0 && littlePosDir == 0 ) {
            littlePosDir = 1;
         }
         if ( bigPosDir ) {
            ++bigPos;
         } else {
            --bigPos;
         }
         if ( littlePosDir ) {
            ++littlePos;
         } else {
            --littlePos;
         }
      }

      const uint8_t val1 = carpet->pot->read() / 4;
      const uint8_t val2 = ( val1 + 128 ) % 255;
      const CRGB clr1 = CHSV( val1, 255, 255 );
      const CRGB clr2 = CHSV( val2, 255, 255 );;
      int diffIndex = scaleTo255( diff, rate, 0 );

      for ( int i = NEO0_OFFSET; i < NEO1_OFFSET; ++i ) {
         int val = scaleTo255( abs( i - littlePos ), SIZEOF_SMALL_NEO, 0 );
         carpet->ropeLeds[ i ] = blend( clr1, clr2, val );
      }
      for ( int i = NEO1_OFFSET; i < NEO3_OFFSET; ++i ) {
         int val = scaleTo255( abs( i - NEO1_OFFSET - bigPos ), NEO3_OFFSET - NEO1_OFFSET, 0 );
         carpet->ropeLeds[ i ] = blend( clr1, clr2, val );
      }
      for ( int i = NEO3_OFFSET; i < NEO4_OFFSET; ++i ) {
         int val = scaleTo255( abs( i - NEO3_OFFSET - littlePos ), NEO4_OFFSET - NEO3_OFFSET, 0 );
         carpet->ropeLeds[ i ] = blend( clr1, clr2, val );
      }
      for ( int i = NEO4_OFFSET; i < NEO6_OFFSET; ++i ) {
         int val = scaleTo255( abs( i - NEO4_OFFSET - littlePos ), NEO6_OFFSET - NEO4_OFFSET, 0 );
         carpet->ropeLeds[ i ] = blend( clr1, clr2, val );
      }
      for ( int i = NEO6_OFFSET; i < NEO7_OFFSET; ++i ) {
         int val = scaleTo255( abs( i - NEO6_OFFSET - bigPos ), NEO7_OFFSET - NEO6_OFFSET, 0 );
         carpet->ropeLeds[ i ] = blend( clr1, clr2, val );
      }
      for ( int i = NEO7_OFFSET; i < NEO7_OFFSET + SIZEOF_SMALL_NEO; ++i ) {
         int val = scaleTo255( abs( i - NEO7_OFFSET - littlePos ), SIZEOF_SMALL_NEO, 0 );
         carpet->ropeLeds[ i ] = blend( clr1, clr2, val );
      }
   }
};
