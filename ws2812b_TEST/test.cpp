/*
###############################################################################
#                                                                             #
# WS2812-RPi                                                                  #
# ==========                                                                  #
# A C++ library for driving WS2812 RGB LED's (known as 'NeoPixels' by         #
#     Adafruit) directly from a Raspberry Pi with accompanying Python wrapper #
# Copyright (C) 2014 Rob Kent                                                 #
#                                                                             #
# This program is free software: you can redistribute it and/or modify        #
# it under the terms of the GNU General Public License as published by        #
# the Free Software Foundation, either version 3 of the License, or           #
# (at your option) any later version.                                         #
#                                                                             #
# This program is distributed in the hope that it will be useful,             #
# but WITHOUT ANY WARRANTY; without even the implied warranty of              #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               #
# GNU General Public License for more details.                                #
#                                                                             #
# You should have received a copy of the GNU General Public License           #
# along with this program.  If not, see <http://www.gnu.org/licenses/>.       #
#                                                                             #
###############################################################################
*/

#include "ws2812-rpi.h"

Color_t setBrightness(Color_t c, float k) {
    return Color_t(c.r*k, c.g*k, c.b*k);
}

int main(int argc, char **argv){
    Color_t c = Color_t(255, 80, 0);
    NeoPixel *n=new NeoPixel(3);

    n->colorWipe(c, 1000);

    for (float k=0; k<1; k+=.01) {
        n->setBrightness(k);
        n->show();
    }

    usleep(1000000);
    for (float k=1; k>0; k-=.01) {
        n->setPixelColor(1, setBrightness(c, k));
        n->show();
    }
    for (float k=0; k<1; k+=.01) {
        n->setPixelColor(1, setBrightness(c, k));
        n->show();
    }

    // while(true) n->effectsDemo();
    // delete n;

    return 0;
}