/*
eviljoelBurningManOutfit - Code for my 2016 Burning Man outfit.
Copyright (C) 2016 Joel Luellwitz

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Adafruit_NeoPixel.h>

const uint8_t PIN_STRIP_0 = 6;

const uint16_t PIXEL_COUNT_SHOULDER_INNER = 10;
const uint16_t PIXEL_COUNT_SHOULDER_OUTER = 14;
const uint16_t PIXEL_COUNT_GRID = 20;
const uint16_t PIXEL_COUNT_VENT = 6;
const uint16_t PIXEL_COUNT_STUD = 4;

const uint16_t PIXEL_COUNT_ALL = (PIXEL_COUNT_SHOULDER_INNER * 2) +
    (PIXEL_COUNT_SHOULDER_OUTER * 2) + PIXEL_COUNT_GRID +
    (PIXEL_COUNT_VENT * 2) + (PIXEL_COUNT_STUD * 2);

Adafruit_NeoPixel neoPixelStrip0 = Adafruit_NeoPixel(PIXEL_COUNT_ALL, PIN_STRIP_0, NEO_GRB + NEO_KHZ800);

// Base color
const uint8_t COLOR_BASE_RED = 32;//255;
const uint8_t COLOR_BASE_GREEN = 8;//61;
const uint8_t COLOR_BASE_BLUE = 32;//255;
const uint32_t COLOR_BASE = neoPixelStrip0.Color(COLOR_BASE_RED, COLOR_BASE_GREEN, COLOR_BASE_BLUE);  // Pink

// Alert color
const uint32_t COLOR_ALERT = neoPixelStrip0.Color(0, 0, 255); //neoPixelStrip0.Color(0, 102, 255);  // Blue

// Black
const uint32_t COLOR_BLACK = neoPixelStrip0.Color(0, 0, 0);

const double VENT_STATE_FADE_FREQUENTLY_CHANCE = .25;
const double VENT_STATE_FADE_ONCE_CHANCE = .75;

const uint8_t GRID_FRONT_FACING_PIXEL_COUNT = 3;
const uint16_t GRID_FRONT_FACING_PIXELS[] = {0, 4, 7};  //{5, 10, 15};

const double GRID_ALERT_ONE_PIXEL_CHANCE = .70;
const double GRID_ALERT_MULTIPLE_PIXELS_CHANCE = .30;
const uint8_t GRID_ALERT_MULTIPLE_PIXEL_COUNT = 3;
const unsigned long GRID_ALERT_MIN_DELAY = 0; //180000;  // 3 min
const unsigned long GRID_ALERT_MAX_DELAY = 150000; //720000;  // 12 min max delay plus min delay.
const unsigned long GRID_ALERT_BLINK_HALF_PERIOD = 500;  // Time in milliseconds
const unsigned long GRID_ALERT_DURATION = 60000;

const double GRID_NEAR_WHITE_ONE_PIXEL_CHANCE = .80;
const double GRID_NEAR_WHITE_THREE_PIXELS_CHANCE = .20;
const uint8_t GRID_NEAR_WHITE_MULTIPLE_PIXEL_COUNT = 3;
const unsigned long GRID_NEAR_WHITE_MIN_DELAY = 5000;  // 5 Seconds
const unsigned long GRID_NEAR_WHITE_MAX_DELAY = 10000;  // 10 seconds max delay plus min delay.
const unsigned long GRID_NEAR_WHITE_FLASH_DURATION = 4000;
const unsigned long GRID_NEAR_WHITE_MULTIPLE_SEPARATION = 1000;  // How often multple flashes start after each other.

const double SHIMMER_COLOR_SHIFT_MULTIPLIER = .50;

const uint16_t SHOULDER_LEAD_SEPARATION = 5;
const unsigned long SHOULDER_CYCLE_TIME = 250;  // Time in milliseconds

const unsigned long VENT_STATE_FADE_MIN_DELAY = 90000;  // 1.5 min
const unsigned long VENT_STATE_FADE_MAX_DELAY = 360000;  // 6 min tops plus the min delay
const unsigned long VENT_CYCLE_TIME = 4000;  // Time in milliseconds
const uint8_t VENT_CYCLE_FREQUENTLY_COUNT = 8;

const uint16_t INDEX_VENT_LEFT_START = 20;  // 6 Pixels
const uint16_t INDEX_VENT_RIGHT_START = 26;  // 6 Pixels
const uint16_t INDEX_STUD_LEFT_START = 32;  // 4 Pixels
const uint16_t INDEX_STUD_RIGHT_START = 36;  // 4 Pixels
const uint16_t INDEX_SHOULDER_INNER_LEFT_START = 40;  // 10 Pixels
const uint16_t INDEX_SHOULDER_OUTER_LEFT_START = 50;  // 14 Pixels
const uint16_t INDEX_SHOULDER_INNER_RIGHT_START = 64;  // 10 Pixels
const uint16_t INDEX_SHOULDER_OUTER_RIGHT_START = 74;  // 14 Pixels
const uint16_t INDEX_GRID_START = 0;  // 20 Pixels

const uint8_t ZONE_VENT_LEFT = 2;
const uint8_t ZONE_VENT_RIGHT = 3;
const uint8_t ZONE_STUD_LEFT = 4;
const uint8_t ZONE_STUD_RIGHT = 5;
const uint8_t ZONE_SHOULDER_INNER_LEFT = 6;
const uint8_t ZONE_SHOULDER_OUTER_LEFT = 7;
const uint8_t ZONE_SHOULDER_INNER_RIGHT = 8;
const uint8_t ZONE_SHOULDER_OUTER_RIGHT = 9;
const uint8_t ZONE_GRID = 10;

const uint8_t VENT_STATE_ON = 100;
const uint8_t VENT_STATE_FADE_ONCE = 101;
const uint8_t VENT_STATE_FADE_FREQUENTLY = 102;

unsigned long ventFadeStartTime;
unsigned long ventFadeEndTime;

uint8_t gridAlertPixelCount;
unsigned long gridAlertStartTime;
unsigned long gridAlertEndTime;
double gridAlertPixelRanks[GRID_FRONT_FACING_PIXEL_COUNT];
uint16_t gridAlertPixelIndexes[GRID_FRONT_FACING_PIXEL_COUNT];
uint8_t gridNearWhitePixelCount;
unsigned long gridNearWhiteStartTime;
unsigned long gridNearWhiteEndTime;
<<<<<<< HEAD
double gridNearWhitePixelRanks[GRID_FRONT_FACING_PIXEL_COUNT];
uint16_t gridNearWhitePixelIndexes[GRID_FRONT_FACING_PIXEL_COUNT];

long max = 0;
=======
uint16_t gridNearWhitePixelRanks[GRID_FRONT_FACING_PIXEL_COUNT];
uint16_t gridNearWhitePixelIndexes[GRID_FRONT_FACING_PIXEL_COUNT];
>>>>>>> 40511f4afc721075d30ad599a2d654cf7c7a12a3

double randomDouble() {
    double randomNumber = random(0, 1569324981) / 1569324981.0;     
    return randomNumber;
}

uint16_t getZoneOffset(uint8_t zone) {

    uint16_t zoneOffset;
  
    if (zone == ZONE_VENT_LEFT) {
        zoneOffset = INDEX_VENT_LEFT_START;
    }
    else if (zone == ZONE_VENT_RIGHT) {
        zoneOffset = INDEX_VENT_RIGHT_START;
    }
    else if (zone == ZONE_STUD_LEFT) {
        zoneOffset = INDEX_STUD_LEFT_START;
    }
    else if (zone == ZONE_STUD_RIGHT) {
        zoneOffset = INDEX_STUD_RIGHT_START;
    }
    else if (zone == ZONE_SHOULDER_INNER_LEFT) {
        zoneOffset = INDEX_SHOULDER_INNER_LEFT_START;
    }
    else if (zone == ZONE_SHOULDER_OUTER_LEFT) {
        zoneOffset = INDEX_SHOULDER_OUTER_LEFT_START;
    }
    else if (zone == ZONE_SHOULDER_INNER_RIGHT) {
        zoneOffset = INDEX_SHOULDER_INNER_RIGHT_START;
    }
    else if (zone == ZONE_SHOULDER_OUTER_RIGHT) {
        zoneOffset = INDEX_SHOULDER_OUTER_RIGHT_START;
    }
    else if (zone == ZONE_GRID) {
        zoneOffset = INDEX_GRID_START;
    }
    
    return zoneOffset;
}

uint8_t getRedColor(uint32_t color) {
     return (uint8_t)(color >> 16);
}

uint8_t getGreenColor(uint32_t color) {
     return (uint8_t)(color >> 8);
}

uint8_t getBlueColor(uint32_t color) {
     return (uint8_t)color;
}

uint32_t getColor(uint8_t zone, uint16_t pixelIndex) {
    uint16_t pixelOffset = getZoneOffset(zone) + pixelIndex;
    return neoPixelStrip0.getPixelColor(pixelOffset);
}

void setColor(uint8_t zone, uint16_t pixelIndex, uint32_t color) {
    uint16_t pixelOffset = getZoneOffset(zone) + pixelIndex;
    neoPixelStrip0.setPixelColor(pixelOffset, color);
}

void animateVents(unsigned long currentFrameTime) {

    // Random fade off then on frequently
    if (currentFrameTime >= ventFadeStartTime) {
	if (currentFrameTime < ventFadeEndTime) {
            
            const unsigned long fadeTime = currentFrameTime - ventFadeStartTime;

            // Set vent color based on a cosine wave.
            const double fadeAmount = cos((fadeTime + 0.0)/ VENT_CYCLE_TIME * 2 * PI) / 2 + .5;

            setVentsFade(fadeAmount);
        }
        else {
            turnVentsOnAndCalculateNextFadeTime(currentFrameTime);
        }
    }
    // Just stay on.
    else {
        // Don't really have to do anything here.
    }
}

void turnVentsOnAndCalculateNextFadeTime(unsigned long currentFrameTime) {

    setVentsFade(1.0);
    
    ventFadeStartTime = currentFrameTime + VENT_STATE_FADE_MIN_DELAY + (VENT_STATE_FADE_MAX_DELAY * randomDouble());

    // Determine the next type of vent fade.
    if (VENT_STATE_FADE_FREQUENTLY_CHANCE < randomDouble()) {
        Serial.print("VENT_STATE_FADE_FREQUENTLY: ");
        Serial.println(ventFadeStartTime);
        ventFadeEndTime = ventFadeStartTime + VENT_CYCLE_TIME;
    }
    else {
        Serial.print("VENT_STATE_FADE_ONCE: ");
        Serial.println(ventFadeStartTime);
        ventFadeEndTime = ventFadeStartTime + (VENT_CYCLE_TIME * VENT_CYCLE_FREQUENTLY_COUNT);
    }
}

void setFadedColor(uint8_t zone, uint16_t pixelIndex, uint32_t color, double amountOn) {
    
    const uint8_t fadedRed = uint8_t(getRedColor(color) * amountOn);
    const uint8_t fadedGreen = uint8_t(getGreenColor(color) * amountOn);
    const uint8_t fadedBlue = uint8_t(getBlueColor(color) * amountOn);
    const uint32_t fadedColor = neoPixelStrip0.Color(fadedRed, fadedGreen, fadedBlue);    

    setColor(zone, pixelIndex, fadedColor);
}

void setVentsFade(double fadeAmount) {
    for (uint16_t pixelIndex = 0; pixelIndex < PIXEL_COUNT_VENT; pixelIndex++) {
        setFadedColor(ZONE_VENT_LEFT, pixelIndex, COLOR_BASE, fadeAmount);
        setFadedColor(ZONE_VENT_RIGHT, pixelIndex, COLOR_BASE, fadeAmount);
    } 
}

void animateSholders(unsigned long currentFrameTime) {
    // Animate inner strips
    animateShouldersSubZone(ZONE_SHOULDER_INNER_LEFT, ZONE_SHOULDER_INNER_RIGHT, currentFrameTime, PIXEL_COUNT_SHOULDER_INNER);
    // Animate outer strips
    animateShouldersSubZone(ZONE_SHOULDER_OUTER_LEFT, ZONE_SHOULDER_OUTER_RIGHT, currentFrameTime, PIXEL_COUNT_SHOULDER_OUTER);
}

/*
 * Animates either the inner or outer shoulder LED string. TODO:
 */
void animateShouldersSubZone(uint8_t leftZone, uint8_t rightZone, unsigned long currentFrameTime, uint16_t pixelCount) {

    const double placeInCycle = (currentFrameTime % SHOULDER_CYCLE_TIME + 0.0) / SHOULDER_CYCLE_TIME * SHOULDER_LEAD_SEPARATION;
    const uint16_t firstFadedLight = uint16_t(placeInCycle);  
    const uint16_t secondFadedLight = uint16_t(placeInCycle + 1) % SHOULDER_LEAD_SEPARATION;

    for (uint16_t pixelIndex = 0; pixelIndex < pixelCount; pixelIndex++) {

        const uint16_t pixelInCycle = pixelIndex % SHOULDER_LEAD_SEPARATION;
        const uint16_t firstPixelOfSeparation = pixelIndex / SHOULDER_LEAD_SEPARATION * SHOULDER_LEAD_SEPARATION;

        // Basically here I am trying to fade two pixels to represent when a pixel should be fully on between two
        //   pixels.
        if (firstFadedLight + firstPixelOfSeparation == pixelIndex) {
            double amountOn = uint16_t(placeInCycle) + 1 - placeInCycle;
            setFadedColor(leftZone, pixelIndex, COLOR_BASE, amountOn);
            setFadedColor(rightZone, pixelIndex, COLOR_BASE, amountOn);
        }
        else if (secondFadedLight + firstPixelOfSeparation == pixelIndex) {
            double amountOn = placeInCycle - uint16_t(placeInCycle);
            setFadedColor(leftZone, pixelIndex, COLOR_BASE, amountOn);
            setFadedColor(rightZone, pixelIndex, COLOR_BASE, amountOn);
        }
        else {
            setColor(leftZone, pixelIndex, COLOR_BLACK);
            setColor(rightZone, pixelIndex, COLOR_BLACK);
        }
    }
}

/*
 * 'Shimmers' the pixels not used in other effects. Shimmering is setting the remaining grid pixels to a somewhat
 *   random deviation of the base color.
 */
void shimmerRemainingPixels(boolean pixelsUsed[]) {
    for (uint16_t pixelIndex = 0; pixelIndex < PIXEL_COUNT_GRID; pixelIndex++) {
        if (pixelsUsed[pixelIndex] == false) {
            uint8_t adjustedRed = uint8_t(min(max(COLOR_BASE_RED + (COLOR_BASE_RED * SHIMMER_COLOR_SHIFT_MULTIPLIER * randomDouble() * (randomDouble() * 2 - 1)), 0), 255));
            uint8_t adjustedGreen = uint8_t(min(max(COLOR_BASE_GREEN + (COLOR_BASE_GREEN * SHIMMER_COLOR_SHIFT_MULTIPLIER * randomDouble() * (randomDouble() * 2 - 1)), 0), 255));
            uint8_t adjustedBlue = uint8_t(min(max(COLOR_BASE_BLUE + (COLOR_BASE_BLUE * SHIMMER_COLOR_SHIFT_MULTIPLIER * randomDouble() * (randomDouble() * 2 - 1)), 0), 255));
            uint32_t adjustedColor = neoPixelStrip0.Color(adjustedRed, adjustedGreen, adjustedBlue);
            setColor(ZONE_GRID, pixelIndex, adjustedColor);
        }
    }
}

/*
 * Sorts the front facing pixels to assist with front facing pixel randomization.
 */
void swapSort(uint16_t indexArray[], double rankArray[]) {
    
    boolean done = false;  // flag to know when we're done sorting              
    while(done == false) {  // simple swap sort, sorts numbers from lowest to highest
        done = true;
        for (uint16_t index = 0; index < (GRID_FRONT_FACING_PIXEL_COUNT - 2); index++) {
            if (rankArray[index] > rankArray[index + 1]) {  // numbers are out of order - swap
                uint16_t swapIndex = indexArray[index + 1];
                double swapRank = rankArray[index + 1];
                indexArray[index + 1] = indexArray[index];
                rankArray[index + 1] = rankArray[index];
                indexArray[index] = swapIndex;
                rankArray[index] = swapRank;
                done = false;
            }
        }
    }
}

/*
 * Determines when and the type of the next grid 'alert'. An 'alert' is essentially blinking red lights.
 */
void determineNextGridAlert(unsigned long currentFrameTime) {
    if (randomDouble() < GRID_ALERT_MULTIPLE_PIXELS_CHANCE) {
        gridAlertPixelCount = GRID_ALERT_MULTIPLE_PIXEL_COUNT;
    }
    else {
        gridAlertPixelCount = 1;
    }

    gridAlertStartTime = currentFrameTime + GRID_ALERT_MIN_DELAY + randomDouble() * GRID_ALERT_MAX_DELAY;
    gridAlertEndTime = gridAlertStartTime + GRID_ALERT_DURATION;
    
    Serial.print("gridAlertStartTime: ");
    Serial.println(gridAlertStartTime);

    for (uint8_t frontFacingPixelIndex = 0; frontFacingPixelIndex < GRID_FRONT_FACING_PIXEL_COUNT; frontFacingPixelIndex++) {
        gridAlertPixelRanks[frontFacingPixelIndex] = randomDouble();
        gridAlertPixelIndexes[frontFacingPixelIndex] = GRID_FRONT_FACING_PIXELS[frontFacingPixelIndex];
    }

    swapSort(gridAlertPixelIndexes, gridAlertPixelRanks);
}

/*
 * Determines when and the type of the next grid white flash.
 */
void determineNextGridNearWhite(unsigned long currentFrameTime) {
    
    if (randomDouble() < GRID_ALERT_MULTIPLE_PIXELS_CHANCE) {
        gridNearWhitePixelCount = GRID_ALERT_MULTIPLE_PIXEL_COUNT;
    }
    else {
        gridNearWhitePixelCount = 1;
    }

    gridNearWhiteStartTime = currentFrameTime + GRID_NEAR_WHITE_MIN_DELAY + randomDouble() * GRID_NEAR_WHITE_MAX_DELAY;
    gridNearWhiteEndTime = gridNearWhiteStartTime + GRID_NEAR_WHITE_FLASH_DURATION + 
        (gridNearWhitePixelCount - 1) * GRID_NEAR_WHITE_MULTIPLE_SEPARATION;

    for (uint8_t frontFacingPixelIndex = 0; frontFacingPixelIndex < GRID_FRONT_FACING_PIXEL_COUNT; frontFacingPixelIndex++) {
        gridNearWhitePixelRanks[frontFacingPixelIndex] = randomDouble();
        gridNearWhitePixelIndexes[frontFacingPixelIndex] = GRID_FRONT_FACING_PIXELS[frontFacingPixelIndex];
    }

    swapSort(gridNearWhitePixelIndexes, gridNearWhitePixelRanks);
}

/*
 * Processes the next frame of the grid. Includes 'alert' flashes, bright white flashes, and a shimmer effect based on the
 *   base color.
 */
void animateGrid(unsigned long currentFrameTime) {

    boolean pixelsUsed[PIXEL_COUNT_GRID];
    for (uint16_t pixelsUsedIndex = 0; pixelsUsedIndex < PIXEL_COUNT_GRID; pixelsUsedIndex++) {
        pixelsUsed[pixelsUsedIndex] = false;
    }

    // Three point alert color or one point alert color.
    if (currentFrameTime >= gridAlertStartTime) {
        
        if (currentFrameTime < gridAlertEndTime) {

            for (uint8_t alertPixelIndex = 0; alertPixelIndex < gridAlertPixelCount; alertPixelIndex++) {
            
                uint16_t gridPixelIndex = gridAlertPixelIndexes[alertPixelIndex];
                
                unsigned long timeInPeriod = (currentFrameTime - gridAlertStartTime) % (2 * GRID_ALERT_BLINK_HALF_PERIOD);

                Serial.print("alert timeInPeriod: ");
                Serial.println(timeInPeriod);

                if (pixelsUsed[gridPixelIndex] == false &&
                        ((alertPixelIndex == 0 && timeInPeriod < GRID_ALERT_BLINK_HALF_PERIOD) ||
                        (alertPixelIndex == 1 && timeInPeriod > GRID_ALERT_BLINK_HALF_PERIOD) ||
                        (alertPixelIndex == 2))) {

                    setColor(ZONE_GRID, gridPixelIndex, COLOR_ALERT);
                    pixelsUsed[gridPixelIndex] = true;
                }
            }
        }
        else {
	    determineNextGridAlert(currentFrameTime);
        }
    }
    
    // Random 3 point or 1 point near white twinkle
    if (currentFrameTime >= gridNearWhiteStartTime) {
        
        if (currentFrameTime < gridNearWhiteEndTime) {

            for (uint8_t nearWhitePixelIndex = 0; nearWhitePixelIndex < gridNearWhitePixelCount; nearWhitePixelIndex++) {
            
                uint16_t gridPixelIndex = gridNearWhitePixelIndexes[nearWhitePixelIndex];
            
                if (pixelsUsed[gridPixelIndex] == false &&
                         gridNearWhiteStartTime + GRID_NEAR_WHITE_MULTIPLE_SEPARATION * nearWhitePixelIndex < currentFrameTime && 
                         currentFrameTime < gridNearWhiteStartTime + GRID_NEAR_WHITE_FLASH_DURATION + 
                             GRID_NEAR_WHITE_MULTIPLE_SEPARATION * nearWhitePixelIndex) {

                    unsigned long adjustedTime = currentFrameTime - (gridNearWhiteStartTime + GRID_NEAR_WHITE_MULTIPLE_SEPARATION * nearWhitePixelIndex);

                    uint8_t adjustedRed = (255 - COLOR_BASE_RED) * sin(adjustedTime * 1.0 / GRID_NEAR_WHITE_FLASH_DURATION * PI) + COLOR_BASE_RED;
                    uint8_t adjustedGreen = (255 - COLOR_BASE_GREEN) * sin(adjustedTime * 1.0 / GRID_NEAR_WHITE_FLASH_DURATION * PI) + COLOR_BASE_GREEN;
                    uint8_t adjustedBlue = (255 - COLOR_BASE_BLUE) * sin(adjustedTime * 1.0 / GRID_NEAR_WHITE_FLASH_DURATION * PI) + COLOR_BASE_BLUE;
                    uint32_t adjustedColor = neoPixelStrip0.Color(adjustedRed, adjustedGreen, adjustedBlue);

                    setColor(ZONE_GRID, gridPixelIndex, adjustedColor);

                    pixelsUsed[gridPixelIndex] = true;
                }
            }
        }
        else {
            determineNextGridNearWhite(currentFrameTime);
        }
    }

    shimmerRemainingPixels(pixelsUsed);
}

/*
 * Determines the next time the grid 'alert' and white flash occurs. Also sets all the grid LEDs
 *   to a somewhat random deviation from the base color. 
 */
void initializeGrid(unsigned long currentFrameTime) {
    
    boolean pixelsUsed[PIXEL_COUNT_GRID];
    for (uint16_t pixelsUsedIndex = 0; pixelsUsedIndex < PIXEL_COUNT_GRID; pixelsUsedIndex++) {
        pixelsUsed[pixelsUsedIndex] = false;
    }

    determineNextGridAlert(currentFrameTime);
    determineNextGridNearWhite(currentFrameTime);

    shimmerRemainingPixels(pixelsUsed);
}

/*
 * Sets the color of LED between the studs to the base color.
 */
void initializeStuds() {

    for (uint16_t pixelIndex = 0; pixelIndex < PIXEL_COUNT_STUD; pixelIndex++) {
        setColor(ZONE_STUD_LEFT, pixelIndex, COLOR_BASE);
        setColor(ZONE_STUD_RIGHT, pixelIndex, COLOR_BASE);
    }
}

/*
 * Initializes the program.
 */
void setup() {
    Serial.begin(9600);
    neoPixelStrip0.begin();

    unsigned long currentFrameTime = millis();
    
    randomSeed(analogRead(0));  // Remove if we finish the pants.

    turnVentsOnAndCalculateNextFadeTime(currentFrameTime);
    animateSholders(currentFrameTime);
    initializeGrid(currentFrameTime);
    initializeStuds();

    neoPixelStrip0.show();
}

/*
 * The main program loop.
 */
void loop() {
    unsigned long currentFrameTime = millis();

    animateVents(currentFrameTime);
    animateSholders(currentFrameTime);
    animateGrid(currentFrameTime);
    // FYI, studs never change.

    neoPixelStrip0.show();
}
