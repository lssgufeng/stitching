//  freak.cpp
//
//	Copyright (C) 2011-2012  Signal processing laboratory 2, EPFL,
//	Kirell Benzi (kirell.benzi@epfl.ch),
//	Raphael Ortiz (raphael.ortiz@a3.epfl.ch)
//	Alexandre Alahi (alexandre.alahi@epfl.ch)
//	and Pierre Vandergheynst (pierre.vandergheynst@epfl.ch)
//
//  Redistribution and use in source and binary forms, with or without modification,
//  are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
//  This software is provided by the copyright holders and contributors "as is" and
//  any express or implied warranties, including, but not limited to, the implied
//  warranties of merchantability and fitness for a particular purpose are disclaimed.
//  In no event shall the Intel Corporation or contributors be liable for any direct,
//  indirect, incidental, special, exemplary, or consequential damages
//  (including, but not limited to, procurement of substitute goods or services;
//  loss of use, data, or profits; or business interruption) however caused
//  and on any theory of liability, whether in contract, strict liability,
//  or tort (including negligence or otherwise) arising in any way out of
//  the use of this software, even if advised of the possibility of such damage.

//#include "precomp.hpp"
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <bitset>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <tmmintrin.h>
#include <string.h>

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "freak.h"

#ifndef CV_PI
    #define CV_PI 3.141592653589793
#endif

namespace cv {

// binary: 10000000 => char: 128 or hex: 0x80
static const __m128i binMask = _mm_set_epi8(0x80, 0x80, 0x80,
                                            0x80, 0x80, 0x80,
                                            0x80, 0x80, 0x80,
                                            0x80, 0x80, 0x80,
                                            0x80, 0x80, 0x80,
                                            0x80);

static const double CV_FREAK_SQRT2 = 1.4142135623731;
static const double CV_FREAK_INV_SQRT2 = 1.0 / CV_FREAK_SQRT2;
static const double CV_FREAK_LOG2 = 0.693147180559945;
static const int CV_FREAK_NB_SCALES = 64;
static const int CV_FREAK_NB_ORIENTATION = 256;
static const int CV_FREAK_NB_POINTS = 43;
static const int CV_FREAK_NB_PAIRS = 512;
static const int CV_FREAK_SMALLEST_KP_SIZE = 7; // smallest size of keypoints
static const int CV_FREAK_NB_ORIENPAIRS = 45;

static const int CV_FREAK_DEF_PAIRS[CV_FREAK_NB_PAIRS] = { // default pairs
                                    404,431,818,511,181,52,311,874,774,543,719,230,417,205,11,
                                    560,149,265,39,306,165,857,250,8,61,15,55,717,44,412,
                                    592,134,761,695,660,782,625,487,549,516,271,665,762,392,178,
                                    796,773,31,672,845,548,794,677,654,241,831,225,238,849,83,
                                    691,484,826,707,122,517,583,731,328,339,571,475,394,472,580,
                                    381,137,93,380,327,619,729,808,218,213,459,141,806,341,95,
                                    382,568,124,750,193,749,706,843,79,199,317,329,768,198,100,
                                    466,613,78,562,783,689,136,838,94,142,164,679,219,419,366,
                                    418,423,77,89,523,259,683,312,555,20,470,684,123,458,453,833,
                                    72,113,253,108,313,25,153,648,411,607,618,128,305,232,301,84,
                                    56,264,371,46,407,360,38,99,176,710,114,578,66,372,653,
                                    129,359,424,159,821,10,323,393,5,340,891,9,790,47,0,175,346,
                                    236,26,172,147,574,561,32,294,429,724,755,398,787,288,299,
                                    769,565,767,722,757,224,465,723,498,467,235,127,802,446,233,
                                    544,482,800,318,16,532,801,441,554,173,60,530,713,469,30,
                                    212,630,899,170,266,799,88,49,512,399,23,500,107,524,90,
                                    194,143,135,192,206,345,148,71,119,101,563,870,158,254,214,
                                    276,464,332,725,188,385,24,476,40,231,620,171,258,67,109,
                                    844,244,187,388,701,690,50,7,850,479,48,522,22,154,12,659,
                                    736,655,577,737,830,811,174,21,237,335,353,234,53,270,62,
                                    182,45,177,245,812,673,355,556,612,166,204,54,248,365,226,
                                    242,452,700,685,573,14,842,481,468,781,564,416,179,405,35,
                                    819,608,624,367,98,643,448,2,460,676,440,240,130,146,184,
                                    185,430,65,807,377,82,121,708,239,310,138,596,730,575,477,
                                    851,797,247,27,85,586,307,779,326,494,856,324,827,96,748,
                                    13,397,125,688,702,92,293,716,277,140,112,4,80,855,839,1,
                                    413,347,584,493,289,696,19,751,379,76,73,115,6,590,183,734,
                                    197,483,217,344,330,400,186,243,587,220,780,200,793,246,824,
                                    41,735,579,81,703,322,760,720,139,480,490,91,814,813,163,
                                    152,488,763,263,425,410,576,120,319,668,150,160,302,491,515,
                                    260,145,428,97,251,395,272,252,18,106,358,854,485,144,550,
                                    131,133,378,68,102,104,58,361,275,209,697,582,338,742,589,
                                    325,408,229,28,304,191,189,110,126,486,211,547,533,70,215,
                                    670,249,36,581,389,605,331,518,442,822
                                   };

struct PatternPoint
{
    float x; // x coordinate relative to center
    float y; // x coordinate relative to center
    float sigma; // Gaussian smoothing sigma
};

struct DescriptionPair
{
    uchar i; // index of the first point
    uchar j; // index of the second point
};

struct OrientationPair
{
    uchar i; // index of the first point
    uchar j; // index of the second point
    int weight_dx; // dx/(norm_sq))*4096
    int weight_dy; // dy/(norm_sq))*4096
};

struct PairStat
{ // used to sort pairs during pairs selection
    double mean;
    int idx;
};

struct sortMean
{
    bool operator()( const PairStat& a, const PairStat& b ) const {
        return a.mean < b.mean;
    }
};

struct FREAKImpl {

    explicit FREAKImpl( bool orientationNormalized_
           , bool scaleNormalized_
           , float patternScale_
           , int nbOctave_
           , const std::vector<int>& vector_
         );

    ~FREAKImpl();
    void compute( const Mat& image, std::vector<KeyPoint>& keypoints, Mat& descriptors ) const;

    /** create the pattern lookup table for all orientation and scale
         * @param filename (optional) vector containing the selected pairs
    */
    void buildPattern( const std::vector<int>& selectedPairs );

    /** draw the description pattern, debug only uncomment if needed */
   // void drawPattern();

    std::vector<int> selectPairs( const std::vector<Mat>& images, std::vector<std::vector<KeyPoint> >& keypoints,
                        const double corrTresh, bool verbose );

    /** compute the intensity of a pattern point (simple mean approximation on a square box)
         * @param image grayscale image
         * @param integral integral image
         * @param kp_x keypoint x coordinate in image (in pixels)
         * @param kp_y keypoint y coordinate in image (in pixels)
         * @param scale scale index in look-up table
         * @param rot orientation index in look-up table
         * @param point point index in look-up table
    */
    uchar meanIntensity( const Mat& image, const Mat& integral, const float kp_x, const float kp_y,
                         const unsigned int scale, const unsigned int rot, const unsigned int point ) const;

    bool orientationNormalized; //true if the orientation is normalized, false otherwise
    bool scaleNormalized; //true if the scale is normalized, false otherwise
    float patternScale; //scaling of the pattern
    int nbOctaves; //number of octaves
    bool extAll; // true if all pairs need to be extracted for pairs selection
    PatternPoint* patternLookup; // look-up table for the pattern points (position+sigma of all points at all scales and orientation)
    int patternSizes[CV_FREAK_NB_SCALES]; // size of the pattern at a specific scale (used to check if a point is within image boundaries)
    DescriptionPair descriptionPairs[CV_FREAK_NB_PAIRS];
    OrientationPair orientationPairs[CV_FREAK_NB_ORIENPAIRS];
};


FREAKImpl::FREAKImpl( bool orientationNormalized, bool scaleNormalized,
              float patternScale, int nbOctaves, const std::vector<int>& selectedPairs )
    : orientationNormalized(orientationNormalized)
    , scaleNormalized(scaleNormalized)
    , patternScale(patternScale)
    , nbOctaves(nbOctaves)
    , extAll(false)
{
    buildPattern(selectedPairs);
}

void FREAKImpl::buildPattern( const std::vector<int>& selectedPairs )
{
    patternLookup = new PatternPoint[CV_FREAK_NB_SCALES*CV_FREAK_NB_ORIENTATION*CV_FREAK_NB_POINTS];
    double scaleStep = pow(2.0, (double)(nbOctaves)/CV_FREAK_NB_SCALES ); // 2 ^ ( (nbOctaves-1) /nbScales)
    double scalingFactor, alpha, beta, theta = 0;

    // pattern definition, radius normalized to 1.0 (outer point position+sigma=1.0)
    const int n[8] = {6,6,6,6,6,6,6,1}; // number of points on each concentric circle (from outer to inner)
    const double bigR(2.0/3.0); // bigger radius
    const double smallR(2.0/24.0); // smaller radius
    const double unitSpace( (bigR-smallR)/21.0 ); // define spaces between concentric circles (from center to outer: 1,2,3,4,5,6)
    // radii of the concentric cirles (from outer to inner)
    const double radius[8] = {bigR, bigR-6*unitSpace, bigR-11*unitSpace, bigR-15*unitSpace, bigR-18*unitSpace, bigR-20*unitSpace, smallR, 0.0};
    // sigma of pattern points (each group of 6 points on a concentric cirle has the same sigma)
    const double sigma[8] = {radius[0]/2.0, radius[1]/2.0, radius[2]/2.0,
                             radius[3]/2.0, radius[4]/2.0, radius[5]/2.0,
                             radius[6]/2.0, radius[6]/2.0
                            };
    // fill the lookup table
    for( int scaleIdx=0; scaleIdx < CV_FREAK_NB_SCALES; ++scaleIdx ) {
        patternSizes[scaleIdx] = 0; // proper initialization
        scalingFactor = pow(scaleStep,scaleIdx); //scale of the pattern, scaleStep ^ scaleIdx

        for( int orientationIdx = 0; orientationIdx < CV_FREAK_NB_ORIENTATION; ++orientationIdx ) {
            theta = double(orientationIdx)* 2*CV_PI/double(CV_FREAK_NB_ORIENTATION); // orientation of the pattern
            int pointIdx = 0;

            for( size_t i = 0; i < 8; ++i ) {
                for( int k = 0 ; k < n[i]; ++k ) {
                    beta = CV_PI/n[i] * (i%2); // orientation offset so that groups of points on each circles are staggered
                    alpha = double(k)* 2*CV_PI/double(n[i])+beta+theta;

                    // add the point to the look-up table
                    PatternPoint& point = patternLookup[ scaleIdx*CV_FREAK_NB_ORIENTATION*CV_FREAK_NB_POINTS+orientationIdx*CV_FREAK_NB_POINTS+pointIdx ];
                    point.x = radius[i] * cos(alpha) * scalingFactor * patternScale;
                    point.y = radius[i] * sin(alpha) * scalingFactor * patternScale;
                    point.sigma = sigma[i] * scalingFactor * patternScale;

                    // adapt the sizeList if necessary
                    const int sizeMax = ceil((radius[i]+sigma[i])*scalingFactor*patternScale) + 1;
                    if( patternSizes[scaleIdx] < sizeMax )
                        patternSizes[scaleIdx] = sizeMax;

                    ++pointIdx;
                }
            }
        }
    }

    // build the list of orientation pairs
    orientationPairs[0].i=0; orientationPairs[0].j=3; orientationPairs[1].i=1; orientationPairs[1].j=4; orientationPairs[2].i=2; orientationPairs[2].j=5;
    orientationPairs[3].i=0; orientationPairs[3].j=2; orientationPairs[4].i=1; orientationPairs[4].j=3; orientationPairs[5].i=2; orientationPairs[5].j=4;
    orientationPairs[6].i=3; orientationPairs[6].j=5; orientationPairs[7].i=4; orientationPairs[7].j=0; orientationPairs[8].i=5; orientationPairs[8].j=1;

    orientationPairs[9].i=6; orientationPairs[9].j=9; orientationPairs[10].i=7; orientationPairs[10].j=10; orientationPairs[11].i=8; orientationPairs[11].j=11;
    orientationPairs[12].i=6; orientationPairs[12].j=8; orientationPairs[13].i=7; orientationPairs[13].j=9; orientationPairs[14].i=8; orientationPairs[14].j=10;
    orientationPairs[15].i=9; orientationPairs[15].j=11; orientationPairs[16].i=10; orientationPairs[16].j=6; orientationPairs[17].i=11; orientationPairs[17].j=7;

    orientationPairs[18].i=12; orientationPairs[18].j=15; orientationPairs[19].i=13; orientationPairs[19].j=16; orientationPairs[20].i=14; orientationPairs[20].j=17;
    orientationPairs[21].i=12; orientationPairs[21].j=14; orientationPairs[22].i=13; orientationPairs[22].j=15; orientationPairs[23].i=14; orientationPairs[23].j=16;
    orientationPairs[24].i=15; orientationPairs[24].j=17; orientationPairs[25].i=16; orientationPairs[25].j=12; orientationPairs[26].i=17; orientationPairs[26].j=13;

    orientationPairs[27].i=18; orientationPairs[27].j=21; orientationPairs[28].i=19; orientationPairs[28].j=22; orientationPairs[29].i=20; orientationPairs[29].j=23;
    orientationPairs[30].i=18; orientationPairs[30].j=20; orientationPairs[31].i=19; orientationPairs[31].j=21; orientationPairs[32].i=20; orientationPairs[32].j=22;
    orientationPairs[33].i=21; orientationPairs[33].j=23; orientationPairs[34].i=22; orientationPairs[34].j=18; orientationPairs[35].i=23; orientationPairs[35].j=19;

    orientationPairs[36].i=24; orientationPairs[36].j=27; orientationPairs[37].i=25; orientationPairs[37].j=28; orientationPairs[38].i=26; orientationPairs[38].j=29;
    orientationPairs[39].i=30; orientationPairs[39].j=33; orientationPairs[40].i=31; orientationPairs[40].j=34; orientationPairs[41].i=32; orientationPairs[41].j=35;
    orientationPairs[42].i=36; orientationPairs[42].j=39; orientationPairs[43].i=37; orientationPairs[43].j=40; orientationPairs[44].i=38; orientationPairs[44].j=41;

    for( unsigned m = CV_FREAK_NB_ORIENPAIRS; m--; ) {
        const float dx = patternLookup[orientationPairs[m].i].x-patternLookup[orientationPairs[m].j].x;
        const float dy = patternLookup[orientationPairs[m].i].y-patternLookup[orientationPairs[m].j].y;
        const float norm_sq = (dx*dx+dy*dy);
        orientationPairs[m].weight_dx = int((dx/(norm_sq))*4096.0+0.5);
        orientationPairs[m].weight_dy = int((dy/(norm_sq))*4096.0+0.5);
    }

    // build the list of description pairs
    std::vector<DescriptionPair> allPairs;
    for( unsigned int i = 1; i < (unsigned int)CV_FREAK_NB_POINTS; ++i ) {
        // (generate all the pairs)
        for( unsigned int j = 0; (unsigned int)j < i; ++j ) {
            DescriptionPair pair = {(uchar)i,(uchar)j};
            allPairs.push_back(pair);
        }
    }
    // Input vector provided
    if( !selectedPairs.empty() ) {
        if( (int)selectedPairs.size() == CV_FREAK_NB_PAIRS ) {
            for( int i = 0; i < CV_FREAK_NB_PAIRS; ++i )
                 descriptionPairs[i] = allPairs[selectedPairs.at(i)];
        }
        else {
            CV_Error(CV_StsVecLengthErr, "Input vector does not match the required size");
        }
    }
    else { // default selected pairs
        for( int i = 0; i < CV_FREAK_NB_PAIRS; ++i )
             descriptionPairs[i] = allPairs[CV_FREAK_DEF_PAIRS[i]];
    }
}

void FREAKImpl::compute( const Mat& image, std::vector<KeyPoint>& keypoints, Mat& descriptors ) const {

#if CV_SSSE3
    register __m128i operand1;
    register __m128i operand2;
    register __m128i workReg;
    register __m128i result128;
#endif

    Mat imgIntegral;
    integral(image, imgIntegral);
    std::vector<int> kpScaleIdx(keypoints.size()); // used to save pattern scale index corresponding to each keypoints
    const std::vector<int>::iterator ScaleIdxBegin = kpScaleIdx.begin(); // used in std::vector erase function
    const std::vector<cv::KeyPoint>::iterator kpBegin = keypoints.begin(); // used in std::vector erase function
    const float sizeCst = CV_FREAK_NB_SCALES/(CV_FREAK_LOG2* nbOctaves );
    uchar pointsValue[CV_FREAK_NB_POINTS];
    int thetaIdx = 0;
    int direction0;
    int direction1;

    // compute the scale index corresponding to the keypoint size and remove keypoints close to the border
    if( scaleNormalized ) {
        for( size_t k = keypoints.size(); k--; ) {
            //Is k non-zero? If so, decrement it and continue"
            kpScaleIdx[k] = max( (int)(log(keypoints[k].size/CV_FREAK_SMALLEST_KP_SIZE)*sizeCst+0.5) ,0);
            if( kpScaleIdx[k] >= CV_FREAK_NB_SCALES )
                kpScaleIdx[k] = CV_FREAK_NB_SCALES-1;

            if( keypoints[k].pt.x <= patternSizes[kpScaleIdx[k]] || //check if the description at this specific position and scale fits inside the image
                 keypoints[k].pt.y <= patternSizes[kpScaleIdx[k]] ||
                 keypoints[k].pt.x >= image.cols-patternSizes[kpScaleIdx[k]] ||
                 keypoints[k].pt.y >= image.rows-patternSizes[kpScaleIdx[k]]
               ) {
                keypoints.erase(kpBegin+k);
                kpScaleIdx.erase(ScaleIdxBegin+k);
            }
        }
    }
    else {
        const int scIdx = max( (int)(1.0986122886681*sizeCst+0.5) ,0);
        for( size_t k = keypoints.size(); k--; ) {
            kpScaleIdx[k] = scIdx; // equivalent to the formule when the scale is normalized with a constant size of keypoints[k].size=3*SMALLEST_KP_SIZE
            if( kpScaleIdx[k] >= CV_FREAK_NB_SCALES ) {
                kpScaleIdx[k] = CV_FREAK_NB_SCALES-1;
            }
            if( keypoints[k].pt.x <= patternSizes[kpScaleIdx[k]] ||
                keypoints[k].pt.y <= patternSizes[kpScaleIdx[k]] ||
                keypoints[k].pt.x >= image.cols-patternSizes[kpScaleIdx[k]] ||
                keypoints[k].pt.y >= image.rows-patternSizes[kpScaleIdx[k]]
               ) {
                keypoints.erase(kpBegin+k);
                kpScaleIdx.erase(ScaleIdxBegin+k);
            }
        }
    }

    // allocate descriptor memory, estimate orientations, extract descriptors
    if( !extAll ) {
        // extract the best comparisons only
        descriptors = cv::Mat::zeros(keypoints.size(), CV_FREAK_NB_PAIRS/8, CV_8U);
#if CV_SSSE3
        __m128i* ptr= (__m128i*) (descriptors.data+(keypoints.size()-1)*descriptors.step[0]);
#else
        std::bitset<CV_FREAK_NB_PAIRS>* ptr = (std::bitset<CV_FREAK_NB_PAIRS>*) (descriptors.data+(keypoints.size()-1)*descriptors.step[0]);
#endif
        for( size_t k = keypoints.size(); k--; ) {
            // estimate orientation (gradient)
            if( !orientationNormalized ) {
                thetaIdx = 0; // assign 0° to all keypoints
                keypoints[k].angle = 0.0;
            }
            else {
                // get the points intensity value in the un-rotated pattern
                for( int i = CV_FREAK_NB_POINTS; i--; ) {
                    pointsValue[i] = meanIntensity(image, imgIntegral, keypoints[k].pt.x,keypoints[k].pt.y, kpScaleIdx[k], 0, i);
                }
                direction0 = 0;
                direction1 = 0;
                for( int m = 45; m--; ) {
                    //iterate through the orientation pairs
                    const int delta = (pointsValue[ orientationPairs[m].i ]-pointsValue[ orientationPairs[m].j ]);
                    direction0 += delta*(orientationPairs[m].weight_dx)/2048;
                    direction1 += delta*(orientationPairs[m].weight_dy)/2048;
                }

                keypoints[k].angle = atan2((float)direction1,(float)direction0)*(180.0/CV_PI);//estimate orientation
                thetaIdx = int(CV_FREAK_NB_ORIENTATION*keypoints[k].angle*(1/360.0)+0.5);
                if( thetaIdx < 0 )
                    thetaIdx += CV_FREAK_NB_ORIENTATION;

                if( thetaIdx >= CV_FREAK_NB_ORIENTATION )
                    thetaIdx -= CV_FREAK_NB_ORIENTATION;
            }
            // extract descriptor at the computed orientation
            for( int i = CV_FREAK_NB_POINTS; i--; ) {
                pointsValue[i] = meanIntensity(image, imgIntegral, keypoints[k].pt.x,keypoints[k].pt.y, kpScaleIdx[k], thetaIdx, i);
            }           
#if CV_SSSE3
            // extracting descriptor by blocks of 128 bits using SSE instructions
            // note that comparisons order is modified in each block (but first 128 comparisons remain globally the same-->does not affect the 128,384 bits segmanted matching strategy)
            int cnt(0);
            for( int n = 4; n-- ; ) {
                result128 = _mm_setzero_si128();
                for( int m = 8; m--; cnt+=16 ) {
                    operand1 = _mm_set_epi8(pointsValue[descriptionPairs[cnt].i],pointsValue[descriptionPairs[cnt+1].i],pointsValue[descriptionPairs[cnt+2].i],pointsValue[descriptionPairs[cnt+3].i],
                                          pointsValue[descriptionPairs[cnt+4].i],pointsValue[descriptionPairs[cnt+5].i],pointsValue[descriptionPairs[cnt+6].i],pointsValue[descriptionPairs[cnt+7].i],
                                          pointsValue[descriptionPairs[cnt+8].i],pointsValue[descriptionPairs[cnt+9].i],pointsValue[descriptionPairs[cnt+10].i],pointsValue[descriptionPairs[cnt+11].i],
                                          pointsValue[descriptionPairs[cnt+12].i],pointsValue[descriptionPairs[cnt+13].i],pointsValue[descriptionPairs[cnt+14].i],pointsValue[descriptionPairs[cnt+15].i]);

                    operand2 = _mm_set_epi8(pointsValue[descriptionPairs[cnt].j],pointsValue[descriptionPairs[cnt+1].j],pointsValue[descriptionPairs[cnt+2].j],pointsValue[descriptionPairs[cnt+3].j],
                                          pointsValue[descriptionPairs[cnt+4].j],pointsValue[descriptionPairs[cnt+5].j],pointsValue[descriptionPairs[cnt+6].j],pointsValue[descriptionPairs[cnt+7].j],
                                          pointsValue[descriptionPairs[cnt+8].j],pointsValue[descriptionPairs[cnt+9].j],pointsValue[descriptionPairs[cnt+10].j],pointsValue[descriptionPairs[cnt+11].j],
                                          pointsValue[descriptionPairs[cnt+12].j],pointsValue[descriptionPairs[cnt+13].j],pointsValue[descriptionPairs[cnt+14].j],pointsValue[descriptionPairs[cnt+15].j]);

                    workReg = _mm_min_epu8(operand1, operand2); // emulated "greater than" for UNSIGNED int
                    workReg = _mm_cmpeq_epi8(workReg, operand2); // emulated "greater than" for UNSIGNED int

                    workReg = _mm_and_si128(_mm_srli_epi16(binMask, m), workReg); // merge the last 16 bits with the 128bits std::vector until full
                    result128 = _mm_or_si128(result128, workReg);
                }
                (*ptr) = result128;
                ++ptr;
            }
            ptr-=8;
#else
            for( int m = CV_FREAK_NB_PAIRS; m--; ) {
                ptr->set(m, pointsValue[descriptionPairs[m].i]>  pointsValue[descriptionPairs[m].j ] );
            }
            --ptr;
#endif
        }
    }
    else { // extract all possible comparisons for selection
        descriptors = cv::Mat::zeros(keypoints.size(), 128, CV_8U);
        std::bitset<1024>* ptr = (std::bitset<1024>*) (descriptors.data+(keypoints.size()-1)*descriptors.step[0]);

        for( size_t k = keypoints.size(); k--; ) {
            //estimate orientation (gradient)
            if( !orientationNormalized ) {
                thetaIdx = 0;//assign 0° to all keypoints
                keypoints[k].angle = 0.0;
            }
            else {
                //get the points intensity value in the un-rotated pattern
                for( int i = CV_FREAK_NB_POINTS;i--; )
                    pointsValue[i] = meanIntensity(image, imgIntegral, keypoints[k].pt.x,keypoints[k].pt.y, kpScaleIdx[k], 0, i);

                direction0 = 0;
                direction1 = 0;
                for( int m = 45; m--; ) {
                    //iterate through the orientation pairs
                    const int delta = (pointsValue[ orientationPairs[m].i ]-pointsValue[ orientationPairs[m].j ]);
                    direction0 += delta*(orientationPairs[m].weight_dx)/2048;
                    direction1 += delta*(orientationPairs[m].weight_dy)/2048;
                }

                keypoints[k].angle = atan2((float)direction1,(float)direction0)*(180.0/CV_PI); //estimate orientation
                thetaIdx = int(CV_FREAK_NB_ORIENTATION*keypoints[k].angle*(1/360.0)+0.5);

                if( thetaIdx < 0 )
                    thetaIdx += CV_FREAK_NB_ORIENTATION;

                if( thetaIdx >= CV_FREAK_NB_ORIENTATION )
                    thetaIdx -= CV_FREAK_NB_ORIENTATION;
            }
            // get the points intensity value in the rotated pattern
            for( int i = CV_FREAK_NB_POINTS; i--; ) {
                pointsValue[i] = meanIntensity(image, imgIntegral, keypoints[k].pt.x,
                                             keypoints[k].pt.y, kpScaleIdx[k], thetaIdx, i);
            }

            int cnt(0);
            for( int i = 1; i < CV_FREAK_NB_POINTS; ++i ) {
                //(generate all the pairs)
                for( int j = 0; j < i; ++j ) {
                    ptr->set(cnt, pointsValue[i]>pointsValue[j] );
                    ++cnt;
                }
            }
            --ptr;
        }
    }
}

FREAKImpl::~FREAKImpl()
{
    delete [] patternLookup;
}

// simply take average on a square patch, not even gaussian approx
uchar FREAKImpl::meanIntensity( const cv::Mat& image, const cv::Mat& integral,
                                                      const float kp_x,
                                                      const float kp_y,
                                                      const unsigned int scale,
                                                      const unsigned int rot,
                                                      const unsigned int point) const {
    // get point position in image
    const PatternPoint& FreakPoint = patternLookup[scale*CV_FREAK_NB_ORIENTATION*CV_FREAK_NB_POINTS + rot*CV_FREAK_NB_POINTS + point];
    const float xf = FreakPoint.x+kp_x;
    const float yf = FreakPoint.y+kp_y;
    const int x = int(xf);
    const int y = int(yf);
    const int& imagecols = image.cols;

    // get the sigma:
    const float radius = FreakPoint.sigma;

    // calculate output:
    int ret_val;
    if( radius < 0.5 ) {
        // interpolation multipliers:
        const int r_x = (xf-x)*1024;
        const int r_y = (yf-y)*1024;
        const int r_x_1 = (1024-r_x);
        const int r_y_1 = (1024-r_y);
        uchar* ptr = image.data+x+y*imagecols;
        // linear interpolation:
        ret_val = (r_x_1*r_y_1*int(*ptr));
        ptr++;
        ret_val += (r_x*r_y_1*int(*ptr));
        ptr += imagecols;
        ret_val += (r_x*r_y*int(*ptr));
        ptr--;
        ret_val += (r_x_1*r_y*int(*ptr));
        return (ret_val+512)/1024;
    }

    // expected case:

    // calculate borders
    const int x_left = int(xf-radius+0.5);
    const int y_top = int(yf-radius+0.5);
    const int x_right = int(xf+radius+1.5);//integral image is 1px wider
    const int y_bottom = int(yf+radius+1.5);//integral image is 1px higher

    ret_val = integral.at<int>(y_bottom,x_right);//bottom right corner
    ret_val -= integral.at<int>(y_bottom,x_left);
    ret_val += integral.at<int>(y_top,x_left);
    ret_val -= integral.at<int>(y_top,x_right);
    ret_val = ret_val/( (x_right-x_left)* (y_bottom-y_top) );
    //~ std::cout<<integral.step[1]<<std::endl;
    return ret_val;
}

// pair selection algorithm from a set of training images and corresponding keypoints
std::vector<int> FREAKImpl::selectPairs(const std::vector<Mat>& images
                                        , std::vector<std::vector<KeyPoint> >& keypoints
                                        , const double corrTresh
                                        , bool verbose )
{
    extAll = true;
    // compute descriptors with all pairs
    Mat descriptors;

    if( verbose )
        std::cout << "Number of images: " << images.size() << std::endl;

    for( size_t i = 0;i < images.size(); ++i ) {
        Mat descriptorsTmp;
        compute(images[i],keypoints[i],descriptorsTmp);
        descriptors.push_back(descriptorsTmp);
    }

    if( verbose )
        std::cout << "number of keypoints: " << descriptors.rows << std::endl;

    //descriptor in floating point format (each bit is a float)
    Mat descriptorsFloat = Mat::zeros(descriptors.rows, 903, CV_32F);

    std::bitset<1024>* ptr = (std::bitset<1024>*) (descriptors.data+(descriptors.rows-1)*descriptors.step[0]);
    for( size_t m = descriptors.rows; m--; ) {
        for( size_t n = 903; n--; ) {
            if( ptr->test(n) == true )
                descriptorsFloat.at<float>(m,n)=1.0;
        }
        --ptr;
    }

    std::vector<PairStat> pairStat;
    for( size_t n = 903; n--; ) {
        // the higher the variance, the better --> mean = 0.5
        PairStat tmp = { fabs( mean(descriptorsFloat.col(n))[0]-0.5 ) ,n};
        pairStat.push_back(tmp);
    }

    std::sort( pairStat.begin(),pairStat.end(), sortMean() );

    std::vector<PairStat> bestPairs;
    for( int m = 0; m < 903; ++m ) {
        if( verbose )
            std::cout << m << ":" << bestPairs.size() << " " << std::flush;
        double corrMax(0);

        for( size_t n = 0; n < bestPairs.size(); ++n ) {
            int idxA = bestPairs[n].idx;
            int idxB = pairStat[m].idx;
            double corr(0);
            // compute correlation between 2 pairs
            corr = fabs(compareHist(descriptorsFloat.col(idxA), descriptorsFloat.col(idxB), CV_COMP_CORREL));

            if( corr > corrMax ) {
                corrMax = corr;
                if( corrMax >= corrTresh )
                    break;
            }
        }

        if( corrMax < corrTresh/*0.7*/ )
            bestPairs.push_back(pairStat[m]);

        if( bestPairs.size() >= 512 ) {
            if( verbose )
                std::cout << m << std::endl;
            break;
        }
    }

    std::vector<int> idxBestPairs;
    if( (int)bestPairs.size() >= CV_FREAK_NB_PAIRS ) {
        for( int i = 0; i < CV_FREAK_NB_PAIRS; ++i )
            idxBestPairs.push_back(bestPairs[i].idx);
    }
    else {
        if( verbose )
            std::cout << "correlation threshold too small (restrictive)" << std::endl;
        CV_Error(CV_StsError, "correlation threshold too small (restrictive)");
    }
    extAll = false;
    return idxBestPairs;
}


/*
void FREAKImpl::drawPattern()
{ // create an image showing the brisk pattern
    Mat pattern = Mat::zeros(1000, 1000, CV_8UC3) + Scalar(255,255,255);
    int sFac = 500 / patternScale;
    for( int n = 0; n < kNB_POINTS; ++n ) {
        PatternPoint& pt = patternLookup[n];
        circle(pattern, Point( pt.x*sFac,pt.y*sFac)+Point(500,500), pt.sigma*sFac, Scalar(0,0,255),2);
        // rectangle(pattern, Point( (pt.x-pt.sigma)*sFac,(pt.y-pt.sigma)*sFac)+Point(500,500), Point( (pt.x+pt.sigma)*sFac,(pt.y+pt.sigma)*sFac)+Point(500,500), Scalar(0,0,255),2);

        circle(pattern, Point( pt.x*sFac,pt.y*sFac)+Point(500,500), 1, Scalar(0,0,0),3);
        std::ostringstream oss;
        oss << n;
        putText( pattern, oss.str(), Point( pt.x*sFac,pt.y*sFac)+Point(500,500), FONT_HERSHEY_SIMPLEX,0.5, Scalar(0,0,0), 1);
    }
    imshow( "FreakDescriptorExtractor pattern", pattern );
    waitKey(0);
}
*/

// -------------------------------------------------
/* FREAK interface implementation */
FREAK::FREAK( bool orientationNormalized, bool scaleNormalized
            , float patternScale, int nbOctave, const std::vector<int>& selectedPairs )
    : impl(new FREAKImpl(orientationNormalized, scaleNormalized, patternScale, nbOctave, selectedPairs))
{
}

FREAK::~FREAK()
{
    delete impl;
}

void FREAK::computeImpl( const Mat& image, std::vector<KeyPoint>& keypoints, Mat& descriptors ) const
{
    if( image.empty() )
        return;
    if( keypoints.empty() )
        return;
    impl->compute(image, keypoints, descriptors);
}

std::vector<int> FREAK::selectPairs( const std::vector<Mat>& images, std::vector<std::vector<KeyPoint> >& keypoints,
                                     const double corrTresh, bool verbose )
{
    return impl->selectPairs(images, keypoints, corrTresh, verbose);
}

int FREAK::descriptorSize() const {
    return CV_FREAK_NB_PAIRS / 8; // descriptor length in bytes
}

int FREAK::descriptorType() const {
    return CV_8U;
}

// TODO
void FREAK::read( const FileNode& ) {}
void FREAK::write( FileStorage& ) const {}

} // END NAMESPACE CV



