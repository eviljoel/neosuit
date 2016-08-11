/*
neosuit - Code for my 2016 Burning Man outfit.
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

const uint8_t PIN_STRIP_SHOULDER = 6;
const uint8_t PIN_STRIP_GRID = 5;
const uint8_t PIN_STRIP_VENT = 3;
const uint8_t PIN_STRIP_STUD = 9;

const uint16_t PIXEL_COUNT_SHOULDER_INNER = 13;
const uint16_t PIXEL_COUNT_SHOULDER_OUTER = 19;
const uint16_t PIXEL_COUNT_GRID = 20;
const uint16_t PIXEL_COUNT_VENT = 18;
const uint16_t PIXEL_COUNT_STUD = 4;

Adafruit_NeoPixel neoPixelStripStud = Adafruit_NeoPixel(PIXEL_COUNT_STUD, PIN_STRIP_STUD, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel neoPixelStripVent = Adafruit_NeoPixel(PIXEL_COUNT_VENT, PIN_STRIP_VENT, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel neoPixelStripGrid = Adafruit_NeoPixel(PIXEL_COUNT_GRID, PIN_STRIP_GRID, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel neoPixelStripShoulder = Adafruit_NeoPixel(PIXEL_COUNT_SHOULDER_INNER + PIXEL_COUNT_SHOULDER_OUTER, PIN_STRIP_SHOULDER, NEO_GRB + NEO_KHZ800);

// Base color (Pink for now. Originally r255, g061, b255 but divided by 8.)
const uint8_t COLOR_BASE_RED = 32;
const uint8_t COLOR_BASE_GREEN = 8;
const uint8_t COLOR_BASE_BLUE = 32;
const uint32_t COLOR_BASE = neoPixelStripGrid.Color(COLOR_BASE_RED, COLOR_BASE_GREEN, COLOR_BASE_BLUE);

// Alert color
const uint32_t COLOR_ALERT = neoPixelStripGrid.Color(0, 255, 0);  // Green

// Black
const uint32_t COLOR_BLACK = neoPixelStripGrid.Color(0, 0, 0);

const double VENT_STATE_FADE_FREQUENTLY_CHANCE = .25;
const double VENT_STATE_FADE_ONCE_CHANCE = .75;

const uint8_t GRID_FRONT_FACING_PIXEL_COUNT = 3;
const uint16_t GRID_FRONT_FACING_PIXELS[] = {0, 4, 7};

const double GRID_ALERT_ONE_PIXEL_CHANCE = .70;
const double GRID_ALERT_MULTIPLE_PIXELS_CHANCE = .30;
const uint8_t GRID_ALERT_MULTIPLE_PIXEL_COUNT = 3;
const unsigned long GRID_ALERT_MIN_DELAY = 180000;  // 3 min
const unsigned long GRID_ALERT_MAX_DELAY = 540000;  // 9 min max delay plus min delay.
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

const uint16_t INDEX_VENT_START = 0;  // 18 Pixels
const uint16_t INDEX_STUD_START = 0;  // 4 Pixels
const uint16_t INDEX_SHOULDER_INNER_START = 0;  // 13 Pixels
const uint16_t INDEX_SHOULDER_OUTER_START = 13;  // 19 Pixels
const uint16_t INDEX_GRID_START = 0;  // 20 Pixels

const uint8_t ZONE_VENT = 2;
const uint8_t ZONE_STUD = 4;
const uint8_t ZONE_SHOULDER_INNER = 6;
const uint8_t ZONE_SHOULDER_OUTER = 7;
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
double gridNearWhitePixelRanks[GRID_FRONT_FACING_PIXEL_COUNT];
uint16_t gridNearWhitePixelIndexes[GRID_FRONT_FACING_PIXEL_COUNT];

double randomDouble() {
    double randomNumber = random(0, 1569324981) / 1569324981.0;     
    return randomNumber;
}

uint16_t getZoneOffset(uint8_t zone) {

    uint16_t zoneOffset;
  
    if (zone == ZONE_VENT) {
        zoneOffset = INDEX_VENT_START;
    }
    else if (zone == ZONE_STUD) {
        zoneOffset = INDEX_STUD_START;
    }
    else if (zone == ZONE_SHOULDER_INNER) {
        zoneOffset = INDEX_SHOULDER_INNER_START;
    }
    else if (zone == ZONE_SHOULDER_OUTER) {
        zoneOffset = INDEX_SHOULDER_OUTER_START;
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
    uint32_t color;
    
    if (zone == ZONE_STUD) {
        color = neoPixelStripStud.getPixelColor(pixelOffset);
    }
    else if (zone == ZONE_VENT) {
        color = neoPixelStripVent.getPixelColor(pixelOffset);
    }
    else if (zone == ZONE_GRID) {
        color = neoPixelStripGrid.getPixelColor(pixelOffset);
    }
    else {
        color = neoPixelStripShoulder.getPixelColor(pixelOffset);
    }
    
    return color;
}

void setColor(uint8_t zone, uint16_t pixelIndex, uint32_t color) {
    uint16_t pixelOffset = getZoneOffset(zone) + pixelIndex;
    
    if (zone == ZONE_STUD) {
        neoPixelStripStud.setPixelColor(pixelOffset, color);
    }
    else if (zone == ZONE_VENT) {
        neoPixelStripVent.setPixelColor(pixelOffset, color);
    }
    else if (zone == ZONE_GRID) {
        neoPixelStripGrid.setPixelColor(pixelOffset, color);
    }
    else {
        neoPixelStripShoulder.setPixelColor(pixelOffset, color);
    }
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
        ventFadeEndTime = ventFadeStartTime + VENT_CYCLE_TIME;
    }
    else {
        ventFadeEndTime = ventFadeStartTime + (VENT_CYCLE_TIME * VENT_CYCLE_FREQUENTLY_COUNT);
    }
}

void setFadedColor(uint8_t zone, uint16_t pixelIndex, uint32_t color, double amountOn) {
    
    const uint8_t fadedRed = uint8_t(getRedColor(color) * amountOn);
    const uint8_t fadedGreen = uint8_t(getGreenColor(color) * amountOn);
    const uint8_t fadedBlue = uint8_t(getBlueColor(color) * amountOn);
    const uint32_t fadedColor = neoPixelStripGrid.Color(fadedRed, fadedGreen, fadedBlue);    

    setColor(zone, pixelIndex, fadedColor);
}

void setVentsFade(double fadeAmount) {
    for (uint16_t pixelIndex = 0; pixelIndex < PIXEL_COUNT_VENT; pixelIndex++) {
        setFadedColor(ZONE_VENT, pixelIndex, COLOR_BASE, fadeAmount);
    } 
}

void animateSholders(unsigned long currentFrameTime) {
    // Animate inner strip
    animateShouldersSubZone(ZONE_SHOULDER_INNER, currentFrameTime, PIXEL_COUNT_SHOULDER_INNER);
    // Animate outer strip
    animateShouldersSubZone(ZONE_SHOULDER_OUTER, currentFrameTime, PIXEL_COUNT_SHOULDER_OUTER);
}

/*
 * Animates either the inner or outer shoulder LED string.
 */
void animateShouldersSubZone(uint8_t zone, unsigned long currentFrameTime, uint16_t pixelCount) {

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
            setFadedColor(zone, pixelIndex, COLOR_BASE, amountOn);
        }
        else if (secondFadedLight + firstPixelOfSeparation == pixelIndex) {
            double amountOn = placeInCycle - uint16_t(placeInCycle);
            setFadedColor(zone, pixelIndex, COLOR_BASE, amountOn);
        }
        else {
            setColor(zone, pixelIndex, COLOR_BLACK);
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
            uint32_t adjustedColor = neoPixelStripGrid.Color(adjustedRed, adjustedGreen, adjustedBlue);
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
        for (uint16_t index = 0; index < (GRID_FRONT_FACING_PIXEL_COUNT - 1); index++) {
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
    
    boolean frontFacingIndexesUsed[GRID_FRONT_FACING_PIXEL_COUNT];
    for (uint16_t frontFacingPixelIndex = 0; frontFacingPixelIndex < GRID_FRONT_FACING_PIXEL_COUNT; frontFacingPixelIndex++) {
        frontFacingIndexesUsed[frontFacingPixelIndex] = false;
    }

    // Three point alert color or one point alert color.
    if (currentFrameTime >= gridAlertStartTime) {
        
        if (currentFrameTime < gridAlertEndTime) {

            for (uint8_t alertPixelIndex = 0; alertPixelIndex < gridAlertPixelCount; alertPixelIndex++) {
                frontFacingIndexesUsed[alertPixelIndex] = true;
            
                uint16_t gridPixelIndex = gridAlertPixelIndexes[alertPixelIndex];
                
                unsigned long timeInPeriod = (currentFrameTime - gridAlertStartTime) % (2 * GRID_ALERT_BLINK_HALF_PERIOD);

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
            
                if (pixelsUsed[gridPixelIndex] == false && frontFacingIndexesUsed[nearWhitePixelIndex] == false &&
                         gridNearWhiteStartTime + GRID_NEAR_WHITE_MULTIPLE_SEPARATION * nearWhitePixelIndex < currentFrameTime && 
                         currentFrameTime < gridNearWhiteStartTime + GRID_NEAR_WHITE_FLASH_DURATION + 
                             GRID_NEAR_WHITE_MULTIPLE_SEPARATION * nearWhitePixelIndex) {

                    unsigned long adjustedTime = currentFrameTime - (gridNearWhiteStartTime + GRID_NEAR_WHITE_MULTIPLE_SEPARATION * nearWhitePixelIndex);

                    uint8_t adjustedRed = (255 - COLOR_BASE_RED) * sin(adjustedTime * 1.0 / GRID_NEAR_WHITE_FLASH_DURATION * PI) + COLOR_BASE_RED;
                    uint8_t adjustedGreen = (255 - COLOR_BASE_GREEN) * sin(adjustedTime * 1.0 / GRID_NEAR_WHITE_FLASH_DURATION * PI) + COLOR_BASE_GREEN;
                    uint8_t adjustedBlue = (255 - COLOR_BASE_BLUE) * sin(adjustedTime * 1.0 / GRID_NEAR_WHITE_FLASH_DURATION * PI) + COLOR_BASE_BLUE;
                    uint32_t adjustedColor = neoPixelStripGrid.Color(adjustedRed, adjustedGreen, adjustedBlue);

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
        setColor(ZONE_STUD, pixelIndex, COLOR_BASE);
    }
}

/*
 * Initializes the program.
 */
void setup() {
    //Serial.begin(9600);  // TODO: Find a faster baud?
    neoPixelStripStud.begin();
    neoPixelStripVent.begin();
    neoPixelStripGrid.begin();
    neoPixelStripShoulder.begin();

    unsigned long currentFrameTime = millis();
    
    randomSeed(analogRead(0));  // Remove if we finish the pants.

    turnVentsOnAndCalculateNextFadeTime(currentFrameTime);
    animateSholders(currentFrameTime);
    initializeGrid(currentFrameTime);
    initializeStuds();

    neoPixelStripStud.show();
    neoPixelStripVent.show();
    neoPixelStripGrid.show();
    neoPixelStripShoulder.show();
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

    neoPixelStripStud.show();
    neoPixelStripVent.show();
    neoPixelStripGrid.show();
    neoPixelStripShoulder.show();
}
