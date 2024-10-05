#include "algorithms.h"

#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <random>



//==============================================================================
// calcIntegralImage(const cv::Mat& image, cv::Mat& integral_image)
//------------------------------------------------------------------------------
// Calculates the integral image from a given input image. 
//
// Do **NOT** use cv::integralImage for this method.
//
// TODO:
//
// - calculate integral image using the relation given in the assignment
//   specification
//
// - remark: the integral image has to be exactly one pixel larger in width and
//           height as the first row and column contain only zero entries.
//
// Parameters:
//
// const cv::Mat& image: The input image. The data type is CV_8UC1 (i.e. uint8_t)
// access the image via image.at<uint8_t>(x,y)
//
// cv::Mat& integral_image: output parameter to store integral image. The data 
//          type is CV_32SC1 (i.e. int32_t).
//==============================================================================
void algorithms::calcIntegralImage(const cv::Mat& image, cv::Mat& integral_image)
{
 
    integral_image = cv::Mat::zeros(image.rows + 1, image.cols + 1, CV_32SC1); 

    for(int y = 1; y < image.rows + 1; y++)
    {
        for(int x = 1; x < image.cols + 1; x++)
        {
            integral_image.at<int32_t>(y, x) = image.at<uint8_t>(y - 1, x - 1) +
            integral_image.at<int32_t>(y - 1, x) + integral_image.at<int32_t>(y, x - 1) - integral_image.at<int32_t>(y - 1, x - 1);
        }
    }
}


//==============================================================================
// calcHessianResponses(const cv::Mat& image_gray,
//                      const cv::Mat& integral_image, 
//                      int border_size,
//                      std::vector<cv::Mat>& Iyy_out, 
//                      std::vector<cv::Mat>& Ixx_out, 
//                      std::vector<cv::Mat>& Ixy_out,
//                      std::vector<cv::Mat>& response_out)
//------------------------------------------------------------------------------
// Computes the Hessian Responses for each octave and level
//
// - forall octaves, forall levels
//
// - calculate Iyy, Ixx and Ixy via box filtering
//
// - determine the hessian-response for each pixel by using the formula provided
//   in the assignment specification
//
// - you only have to do the calculations for Iyy, Ixx, Ixy, and the
//   hessian-response for every delta-pixel,
//   the rest of the pixels should simply remain zero - so the output image for
//   Iyy, Ixx, Ixy and the hessian-response has the same size as the input image
//
// - store the outputs of Iyy, Ixx, Ixy and the hessian response in the 
//   {Iyy,Ixx,Ixy,response}_out vectors.
//
// - Note that the size of the Ixx,Iyy,Ixy,response matrices is the same 
//   size as image_gray.
//
// - You are allowed to define yourself some helper functions for calculating Iyy, Ixx and Ixy 
//   However **only** modify this file here (algorithms.cpp) !!
//
// - Hint: You can use .clone() when calling .push_back()
//
// Parameters:
//
// const cv::Mat& image_gray: The grayscale image (same size as input image _without_ padding)
//
// const cv::Mat& integral_image: The integral image (with padding).
//
// const int border_size: input parameter used for padding the input image
//
// std::vector<cv::Mat>& Iyy_out: output parameter for storing all Iyy images
//
// std::vector<cv::Mat>& Ixx_out: output parameter for storing all Ixx images
//
// std::vector<cv::Mat>& Ixy_out: output parameter for storing all Ixy images
//
// std::vector<cv::Mat>& response_out: output parameter for storing all the 
//                                 hessian-response images
//==============================================================================
void algorithms::calcHessianResponses(const cv::Mat& image_gray,
                          const cv::Mat& integral_image,
                          const int border_size,
                          std::vector<cv::Mat>& Iyy_out,
                          std::vector<cv::Mat>& Ixx_out,
                          std::vector<cv::Mat>& Ixy_out, 
                          std::vector<cv::Mat>& response_out)
{
    // USE these values !!
    const int num_octaves = 4;
    const int num_levels = 4;
    const float w_const = 0.912;

    for (int octave = 1; octave <= num_octaves; octave++) {
        for (int level = 1; level <= num_levels; level++) {
            int s = pow(2,octave)* level + 1;
            int delta = pow(2,octave-1);

            cv::Mat Ixx = cv::Mat::zeros(image_gray.size(), CV_32FC1);
            cv::Mat Iyy = cv::Mat::zeros(image_gray.size(), CV_32FC1);
            cv::Mat Ixy = cv::Mat::zeros(image_gray.size(), CV_32FC1);
            cv::Mat response = cv::Mat::zeros(image_gray.size(), CV_32FC1);

           

            for (int x = border_size + 1; x < image_gray.rows + border_size + 1; x += delta) {
                

                for (int y = border_size + 1; y < image_gray.cols + border_size + 1; y += delta) {
                    float x_offset = (s - 1) / 2;
                    float y_offset = (s -1);

              
                    int A1 = integral_image.at<int>(x - x_offset - s, y - y_offset ) 
                           - integral_image.at<int>(x - s - x_offset , y + y_offset + 1 ) 
                           - integral_image.at<int>(x + s + x_offset + 1, y - y_offset) 
                           + integral_image.at<int>(x + x_offset + s + 1, y + s );

                    int A2 = integral_image.at<int>(x - x_offset , y - y_offset)
                           - integral_image.at<int>(x - x_offset , y + y_offset + 1) 
                           - integral_image.at<int>(x + x_offset + 1, y - y_offset ) 
                           + integral_image.at<int>(x + x_offset + 1, y + y_offset + 1);
                    Iyy.at<float>(x- border_size - 1, y- border_size - 1) = A1 - 3 * A2;

                    std::swap(x_offset, y_offset);

                    int B1 = integral_image.at<int32_t>(x - x_offset, y - y_offset - s) 
                           - integral_image.at<int32_t>(x - x_offset,  y + y_offset + s + 1) 
                           - integral_image.at<int32_t>(x + s, y - y_offset - s) 
                           + integral_image.at<int32_t>(x + s, y + y_offset + s + 1);

                    int B2 = integral_image.at<int32_t>(x - x_offset, y - y_offset) 
                           - integral_image.at<int32_t>(x - x_offset, y + y_offset + 1) 
                           - integral_image.at<int32_t>(x + s, y - y_offset) 
                           + integral_image.at<int32_t>(x + s, y + y_offset + 1);
                    Ixx.at<float>(x - border_size - 1, y - border_size - 1) = B1 - 3 * B2;

                    int C1 = integral_image.at<int32_t>(x - s, y - s) 
                           - integral_image.at<int32_t>(x - s, y) 
                           - integral_image.at<int32_t>(x, y - s) 
                           + integral_image.at<int32_t>(x, y);
                    int C2 = integral_image.at<int32_t>(x - s, y + 1) 
                           - integral_image.at<int32_t>(x - s, y + s + 1) 
                           - integral_image.at<int32_t>(x, y + 1) 
                           + integral_image.at<int32_t>(x, y + s +1);
                    int C3 = integral_image.at<int32_t>(x + 1, y - s) 
                           - integral_image.at<int32_t>(x + 1, y) 
                           - integral_image.at<int32_t>(x + s + 1, y - s) 
                           + integral_image.at<int32_t>(x + s +1, y);
                    int C4 = integral_image.at<int32_t>(x + 1, y + 1) 
                           - integral_image.at<int32_t>(x + 1, y + s + 1) 
                           - integral_image.at<int32_t>(x + s + 1, y + 1) 
                           + integral_image.at<int32_t>(x + s + 1, y + s + 1);
                    Ixy.at<float>(x - border_size - 1, y - border_size - 1) = C2 + C3 - C1 - C4;

                    float detH = (Ixx.at<float>(x - border_size - 1, y - border_size - 1) * Iyy.at<float>(x - border_size - 1, y- border_size - 1) 
                    - (w_const * Ixy.at<float>(x - border_size - 1, y - border_size - 1)) * (w_const * Ixy.at<float>(x - border_size - 1, y - border_size - 1)))/ pow(s, 4);
                    response.at<float>(x - border_size - 1, y - border_size - 1) = detH;
                }
            }

            Ixx_out.push_back(Ixx.clone());
            Iyy_out.push_back(Iyy.clone());
            Ixy_out.push_back(Ixy.clone());
            response_out.push_back(response.clone());
        }
    }
}




bool isLocalMaxima(const std::vector<cv::Mat>& responses, float currentValue, int x, int y,  int delta, int index) 
{
    
    cv::Mat response = responses[index];
    for (int i = -delta; i <= delta; i += delta)
    {
        for (int j = -delta; j <= delta; j += delta)
        {
            if (((x + j) < 0) || //check for out of bounds
            ((y + i) < 0) || 
            ((x + j) >= response.cols) 
            || ((y + i) >= response.rows))
            {
                continue;
            }

            if (response.at<float>(y + i, x + j) > currentValue||
                responses[index - 1].at<float>(y + i, x + j) > currentValue ||
                responses[index + 1].at<float>(y + i, x + j) > currentValue )
            {
                return false;
            }
        }
    }
    
    return true;
}

//==============================================================================
// calcNmsAndSurfFeatures(const cv::Mat& image_gray,
//                        const cv::Mat& integral_image, 
//                        int border_size,
//                        const std::vector<cv::Mat>& responses, 
//                        std::vector<cv::KeyPoint>& features,
//                        std::vector<cv::Mat>& descriptors) 
//------------------------------------------------------------------------------
// Performs Non Maxima Suppression (NMS). For each local maxima creates a 
// Feature Point and Descriptor.
//
// - for all octaves, for all levels 2..3:
//
// - check if the corresponding hessian response is larger than the threshold
//   and if the point is a local maxima in a 3x3x3 neighborhood.
//
// - if the point is a local maxima, add a SURF keypoint to features 
//   (with the correct parameters, see assignment sheet) and compute
//   the descriptor with addSurfDescriptor(...)
//
// - set the size parameter of the cv::KeyPoint class to sigma (formula from
//   assignment specification; before rounding)
//
// - call addSurfDescriptor(....) with a rounded sigma (as the parameter here
//   is an integer). use std::round(...) for rounding here. 
//   Round only the parameter WHILE calling the function !!
//
// - Hint: std::copy(...) and std::back_inserter(...) can be helpful
//
// You can write a helper function for checking the local maxima, however only defined 
// and used inside this file (algorithms.cpp) !!
//
// Parameters:
//
// const cv::Mat& image_gray: The grayscale image (same size as input image _without_ padding)
//
// const cv::Mat& integral_image: The integral image (with padding)
//
// int border_size: input parameter used for padding the input image
//
// int threshold: the threshold used for the hessian response
//
// const std::vector<cv::Mat>& responses: list of all hessian responses 
//                               (4 octaves with 4 levels each -> size: 4*4=16)
//
// std::vector<cv::KeyPoint>& features: output paramter for storing all keypoints
//
// std::vector<cv::Mat>& descriptors: output parameter for storing all descriptors
//==============================================================================
void algorithms::calcNmsAndSurfFeatures(const cv::Mat &image_gray,
                            const cv::Mat &integral_image,
                            int border_size,
                            float threshold,
                            const std::vector<cv::Mat> &responses,
                            std::vector<cv::KeyPoint>& features,
                            cv::Mat& descriptors)
{
    // USE these values !!
    const int num_octaves = 4;
    const int num_levels = 4;

    for (int o = 1; o <= num_octaves; o++) 
    {
        int delta = std::pow(2, o - 1);
        for (int level = 2; level <= num_levels - 1; ++level)
        {
            int index = ((o - 1) * num_levels) + (level - 1);
            int filterSize = (std::pow(2, o) * level + 1);
            for (int y = 0; y < image_gray.rows; y += delta) 
            {
                for (int x = 0; x < image_gray.cols; x += delta) 
                {
                    cv::Mat currentLevelResponse = responses[index];
                    float currentValue = currentLevelResponse.at<float>(y, x);
                    if (isLocalMaxima(responses, currentValue, x, y, delta, index) && currentValue > threshold) 
                    {
                        float sigma = 0.4f * filterSize;
                        cv::KeyPoint keyPoint(x, y, sigma);
                        int rounded_sigma = std::round(sigma);
                        features.push_back(keyPoint);
                        addSurfDescriptor(integral_image, border_size, keyPoint, descriptors, rounded_sigma, filterSize);
                    }
                }
            }
        }
    }
}



//==============================================================================
// calcSurfFeatures(const cv::Mat &image, std::vector <KeyPoint> &features, 
//                   cv::Mat &integral_out, cv::Mat &Iyy_out, cv::Mat &Ixx_out, 
//                   cv::Mat &Ixy_out)
//------------------------------------------------------------------------------
// Determines image keypoints using SURF algorithm.
//
// TODO:
//
// - convert input image to grayscale
// - pad input image using copyMakeBorder(...) with parameter BORDER_REPLICATE
//   and border_size on all 4 sides
//
// - calculate the integral image, do **not** use the OpenCV function, instead
//   implement the functionality in the provided
//   function body above and call this function here (use calcIntegralImage)
//
// - compute the hessian responses (use calcHessianResponses)
//
// - perform Non Maxima Suppression (NMS) and compute the SURF descriptors
//   and keypoints using calcNmsAndSurfFeatures(...)
//
// - implement the above function stubs first before you implement this function!
// - implement the PDF sections from 2.2 until 2.4 first, corresponding to the functions:
//   calcIntegralImage(...), calcHessianResponses(...), calcNmsAndSurfFeatures(...)
//   -> You will need those all for implementing the SURF features !!
//
// Hint: You can use .clone() on the result stored after calcIntegralImage(...)
//
// After implementing the previous stubs, this function here should be pretty straightforward!
//
// Parameters:
//
// const cv::Mat& image: The input image
//
// const int border_size: input parameter used for padding the input image
//
// const float threshold: threshold used for filtering the hessian-response
//
// const KeyPoint& kp: output parameters to store keypoints
//
// cv::Mat &descriptors: output parameter for descriptor vectors
//
// cv::Mat& integral_out: output parameter for storing integral image
//
// std::vector<cv::Mat>& Iyy_out: output parameter for storing all Iyy images
//
// std::vector<cv::Mat>& Ixx_out: output parameter for storing all Ixx images
//
// std::vector<cv::Mat>& Ixy_out: output parameter for storing all Ixy images
//
// std::vector<cv::Mat>& response_out: output parameter for storing all the 
//                 hessian-response images
//==============================================================================
void algorithms::calcSurfFeatures(const cv::Mat& image, const int border_size,
                      const float threshold, std::vector<cv::KeyPoint>& features,
                      cv::Mat& descriptors, cv::Mat& integral_out, 
                      std::vector<cv::Mat>& Iyy_out,
                      std::vector<cv::Mat>& Ixx_out,
                      std::vector<cv::Mat>& Ixy_out, 
                      std::vector<cv::Mat>& response_out)
{
    Iyy_out.clear();
    Ixx_out.clear();
    Ixy_out.clear();
    response_out.clear();

    integral_out = cv::Mat::zeros(image.rows + (border_size * 2) + 1,
                              image.cols + (border_size * 2) + 1, CV_32SC1);

    cv::Mat image_gray;
    cv::cvtColor(image, image_gray, cv::COLOR_BGR2GRAY);
    

    cv::Mat padded_image;
    cv::copyMakeBorder(image_gray, padded_image, border_size, border_size, border_size, border_size, cv::BORDER_REPLICATE);

    calcIntegralImage(padded_image, integral_out);

    calcHessianResponses(image_gray, integral_out, border_size, Iyy_out, Ixx_out, Ixy_out, response_out);

    calcNmsAndSurfFeatures(image_gray, integral_out, border_size, threshold, response_out, features, descriptors);
}

int helper(const cv::Mat& integral_image, int xA, int yA, int xB, int yB, int xC, int yC, int xD, int yD) {
    return integral_image.at<int>(yD, xD) + integral_image.at<int>(yA, xA) -
           integral_image.at<int>(yB, xB) - integral_image.at<int>(yC, xC);
}

//==============================================================================
// void calcWeightedDerivative(const cv::Mat& integral_image, const int border_size,
//                             const KeyPoint& kp, int sigma, int s, 
//                             cv::Mat &weighted_derivative_x, 
//                             cv::Mat &weighted_derivative_y)
//------------------------------------------------------------------------------
// Calculates gaussian-weighted first order derivatives in a 20x20 window around keypoint kp
//
// TODO:
//
// - extract 20x20 window around descriptor
//
// - calculate Ix and Iy using box filtering
//
// - weight Ix and Iy with a Gaussian kernel
//
// - use the output parameters weighted_derivative_x and weighted_derivative_y
//   to store the results for further use in addSurfDescriptor(...)
//
// - you are allowed to define yourself helper functions for calculating Ix and Iy
//   however only define and use them inside this file (algorithms.cpp) !!
//
// Parameters:
//
// const cv::Mat& integral_image: The integral image
//
// const int border_size: border size used in calcSurfFeatures(...) for padding
//                        the input image 
//
// const KeyPoint& kp: keypoint for which the weighted
//                     derivatives should be calculated 
//
// int sigma: calculation parameter sigma 
//
// int s: calculation parameter s 
//
// weighted_derivative_x: output parameter for weighted derivative in x 
//                        direction 
//
// weighted_derivative_y: output parameter for weighted derivative in y 
//                        direction
//==============================================================================
void algorithms::calcWeightedDerivative(const cv::Mat& integral_image, const int border_size,
                            const cv::KeyPoint& kp, int sigma, int s,
                            cv::Mat& weighted_derivative_x,
                            cv::Mat& weighted_derivative_y)
{
   int t = std::round(0.8 * s);

    std::vector<int> h(20), w(20);
    for (int i = -10; i < 10; ++i) {
        h[i + 10] = i;
        w[i + 10] = i;
    }

    cv::Mat gauss_vector = cv::getGaussianKernel(20, 3.3f * static_cast<float>(sigma), CV_32FC1);
    cv::Mat G_matrix = gauss_vector * gauss_vector.t();

    for (int y = 0; y < 20; ++y) 
    {
        for (int x = 0; x < 20; ++x) 
        {
            int x0 = std::round((w[x] + 0.5) * sigma + kp.pt.x) + border_size + 1;
            int y0 = std::round((h[y] + 0.5) * sigma + kp.pt.y) + border_size + 1;

            // Calculate the coordinates for the red section of the filter for Ix
            int A_x = x0 - t, A_y = y0 - t;
            int B_x = x0 - 1, B_y = y0 - t;
            int C_x = x0 - t, C_y = y0 + t;
            int D_x = x0 - 1, D_y = y0 + t;

            if (A_x < 0) { 
                A_x = 0; 
            }
            if (A_y < 0) { 
                A_y = 0; 
            }
            if (B_x >= integral_image.cols) { 
                B_x = integral_image.cols - 1; 
            }
            if (B_y < 0) { 
                B_y = 0; 
            }
            if (C_x < 0) { 
                C_x = 0; 
            }
            if (C_y >= integral_image.rows) { 
                C_y = integral_image.rows - 1; 
            }
            if (D_x >= integral_image.cols) { 
                D_x = integral_image.cols - 1; 
            }
            if (D_y >= integral_image.rows) { 
                D_y = integral_image.rows - 1; 
            }

            float negative_sum = helper(integral_image, A_x, A_y, B_x + 1, B_y, C_x, C_y + 1, D_x + 1, D_y + 1);

            A_x = x0 + 1, A_y = y0 - t;
            B_x = x0 + t, B_y = y0 - t;
            C_x = x0 + 1, C_y = y0 + t;
            D_x = x0 + t, D_y = y0 + t;

            if (A_x < 0) { 
                A_x = 0; 
            }
            if (A_y < 0) { 
                A_y = 0; 
            }
            if (B_x >= integral_image.cols) { 
                B_x = integral_image.cols - 1; 
            }
            if (B_y < 0) { 
                B_y = 0; 
            }
            if (C_x < 0) { 
                C_x = 0; 
            }
            if (C_y >= integral_image.rows) { 
                C_y = integral_image.rows - 1; 
            }
            if (D_x >= integral_image.cols) { 
                D_x = integral_image.cols - 1; 
            }
            if (D_y >= integral_image.rows) { 
                D_y = integral_image.rows - 1; 
            }

            float positive_sum = helper(integral_image, A_x, A_y, B_x + 1, B_y, C_x, C_y + 1, D_x + 1, D_y + 1);

            float Ix = positive_sum - negative_sum;
            weighted_derivative_x.at<float>(y, x) = Ix * G_matrix.at<float>(y, x);

            A_x = x0 - t, A_y = y0 - t;
            B_x = x0 + t, B_y = y0 - t;
            C_x = x0 - t, C_y = y0 - 1;
            D_x = x0 + t, D_y = y0 - 1;

            if (A_x < 0) { 
                A_x = 0; 
            }
            if (A_y < 0) { 
                A_y = 0; 
            }
            if (B_x >= integral_image.cols) { 
                B_x = integral_image.cols - 1; 
            }
            if (B_y < 0) { 
                B_y = 0; 
            }
            if (C_x < 0) { 
                C_x = 0; 
            }
            if (C_y >= integral_image.rows) { 
                C_y = integral_image.rows - 1; 
            }
            if (D_x >= integral_image.cols) { 
                D_x = integral_image.cols - 1; 
            }
            if (D_y >= integral_image.rows) { 
                D_y = integral_image.rows - 1; 
            }

            negative_sum = helper(integral_image, A_x, A_y, B_x + 1, B_y, C_x, C_y + 1, D_x + 1, D_y + 1);

            A_x = x0 - t, A_y = y0 + 1;
            B_x = x0 + t, B_y = y0 + 1;
            C_x = x0 - t, C_y = y0 + t;
            D_x = x0 + t, D_y = y0 + t;

            if (A_x < 0) { 
                A_x = 0; 
            }
            if (A_y < 0) { 
                A_y = 0; 
            }
            if (B_x >= integral_image.cols) { 
                B_x = integral_image.cols - 1; 
            }
            if (B_y < 0) { 
                B_y = 0; 
            }
            if (C_x < 0) { 
                C_x = 0; 
            }
            if (C_y >= integral_image.rows) { 
                C_y = integral_image.rows - 1; 
            }
            if (D_x >= integral_image.cols) { 
                D_x = integral_image.cols - 1; 
            }
            if (D_y >= integral_image.rows) { 
                D_y = integral_image.rows - 1; 
            }

            positive_sum = helper(integral_image, A_x, A_y, B_x + 1, B_y, C_x, C_y + 1, D_x + 1, D_y + 1);

            float Iy = positive_sum - negative_sum;
            weighted_derivative_y.at<float>(y, x) = Iy * G_matrix.at<float>(y, x);
        }
    }
}


//==============================================================================
// addSurfDescriptor(const cv::Mat& integral_image, const KeyPoint& kp, cv::Mat&
//                   descriptors, int sigma, int s)
//------------------------------------------------------------------------------
// Calculates SURF descriptor vector for a given keypoint.
//
// TODO:
//
// - implement and call the function calcWeightedDerivative(...) to calculate
//   gaussian-weighted x- and y- derivatives in a 20x20 window around the 
//   keypoint
//
// - split 20x20 window into 16 5x5 sub-regions
//
// - calculate S_x, S_y, A_x, A_y as described in the assignment sheet and
//   construct the descriptor vector
//
// You can use abs(...) for calculating the absolute values (the sums for A_xk, A_yk)
//
// Parameters:
//
// const cv::Mat& integral_image: The integral image
//
// const int border_size: border size used in calcSurfFeatures(...) for padding
//                        the input image 
//
// const KeyPoint& kp: keypoint for which the descriptor should be calculated 
//
// cv::Mat& descriptors: add the calculated descriptor to this matrix using 
//                   push_back(...) 
//
// int sigma: calculation parameter sigma 
//
// int s: calculation parameter s
//==============================================================================
void algorithms::addSurfDescriptor(const cv::Mat& integral_image, const int border_size,
                    const cv::KeyPoint& kp, cv::Mat& descriptors, int sigma, int s)
{
    cv::Mat weighted_derivative_x = cv::Mat::zeros(20, 20, CV_32FC1);
    cv::Mat weighted_derivative_y = cv::Mat::zeros(20, 20, CV_32FC1);

    calcWeightedDerivative(integral_image, border_size, kp, sigma, s, weighted_derivative_x, weighted_derivative_y);

    std::vector<float> surf_descriptor(64, 0.0f);

    cv::Mat abs_weighted_derivative_x, abs_weighted_derivative_y;
    cv::absdiff(weighted_derivative_x, cv::Scalar(0), abs_weighted_derivative_x);
    cv::absdiff(weighted_derivative_y, cv::Scalar(0), abs_weighted_derivative_y);

    for (int row = 0; row < 4; ++row) 
    {
        for (int col = 0; col < 4; ++col) 
        {
            cv::Rect sub_region(col * 5, row * 5, 5, 5);

            float sum_derivative_x = cv::sum(weighted_derivative_x(sub_region))[0];
            float sum_derivative_y = cv::sum(weighted_derivative_y(sub_region))[0];
            float sum_abs_derivative_x = cv::sum(abs_weighted_derivative_x(sub_region))[0];
            float sum_abs_derivative_y = cv::sum(abs_weighted_derivative_y(sub_region))[0];

            int descriptor_idx = (row * 4 + col) * 4;
            surf_descriptor[descriptor_idx] = sum_derivative_x;
            surf_descriptor[descriptor_idx + 1] = sum_derivative_y;
            surf_descriptor[descriptor_idx + 2] = sum_abs_derivative_x;
            surf_descriptor[descriptor_idx + 3] = sum_abs_derivative_y;
        }
    }

    cv::Mat descriptor_mat(1, 64, CV_32FC1, surf_descriptor.data());
    cv::normalize(descriptor_mat, descriptor_mat);

    descriptors.push_back(descriptor_mat.clone());
}



//==============================================================================
// matchFeatures(const vector<KeyPoint>& features, const cv::Mat& descriptors,
//     const vector<KeyPoint>& new_features, const cv::Mat& new_descriptors,
//     vector<Point2f>& good_matches_source, vector<Point2f>&
//     good_matches_target, DescriptorMatcher& matcher)
//------------------------------------------------------------------------------
// Calculates matches between source and target feature descriptors
//
// TODO:
//
// - calculate matches between source_descriptors and target_descriptors by
//   calling matcher.knnMatch(...)
//
// - discard matches where the distance of the first match is less than
//   match_distance_factor times distance of the second match
//
// - insert the remaining features into the output variables good_matches_source
//   and good_matches_target by using DMatch::queryIdy and DMatch::trainIdx of
//   the first entry of each match pair to index the KeyPoint from
//   source_features and target_features
//
// Parameters:
//
// const vector<KeyPoint>& source_features: Features of the source image
//
// const cv::Mat& source_descriptors: Descriptor vectors of the source image
//
// const vector<KeyPoint>& target_features: Features of the target image
//
// const cv::Mat& target_descriptors: Descriptor vectors of the target image
//
// vector<Point2f>& good_matches_source: Points in the source image that have a
//                                       correspondence in the target image.
//
// vector<Point2f>& good_matches_target: Points in the target image that have a 
//                                       correspondence in the source image.
//
// const DescriptorMatcher& matcher: Used for finding matches between
//                                   source_descriptor and target_descriptor
//
// const float match_distance_factor: used for the distance check
//==============================================================================
void algorithms::matchFeatures(const std::vector<cv::KeyPoint>& source_features,
                   const cv::Mat& source_descriptors,
                   const std::vector<cv::KeyPoint>& target_features,
                   const cv::Mat& target_descriptors,
                   std::vector<cv::Point2f>& good_matches_source,
                   std::vector<cv::Point2f>& good_matches_target,
                   const cv::DescriptorMatcher& matcher,
                   const float match_distance_factor)
{
    const int k = 2;

    good_matches_source.clear();
    good_matches_target.clear();

    std::vector<std::vector<cv::DMatch>> knn_matches;
    
    matcher.knnMatch(source_descriptors, target_descriptors, knn_matches, k);

    for (const auto& match : knn_matches)
    {
        if (match.size() == k)
        {
            const cv::DMatch& best_match = match[0];
            const cv::DMatch& second_best_match = match[1];

            if (best_match.distance < match_distance_factor * second_best_match.distance)
            {
                good_matches_source.push_back(source_features[best_match.queryIdx].pt);
                good_matches_target.push_back(target_features[best_match.trainIdx].pt);
            }
        }
    }
}



//==============================================================================
// cv::Mat calculateHomography(const vector<Point2f>& good_matches_source,
//     const vector<Point2f>& good_matches_target
//------------------------------------------------------------------------------
// Calculates a homography between a number of point correspondences
//
// TODO:
//
// - Calculate the optimal homography between good_matches_source and
//   good_matches_object by using the OpenCV function cv::findHomography with
//   the parameter CV_RANSAC
//
// - Convert the calculated homography to CV_32F
//
// - Hint: You can use std::default_random_engine and std::uniform_int_distribution<int>
//
// Parameters:
//
// const vector<Point2f>& good_matches_source: Points in the source image that
//                                             have a correspondence in the 
//                                             target image 
//
// const vector<Point2f>& good_matches_object: Points in the target image that 
//                                             have a correspondence in the 
//                                             source image
//
// int ransac_iterations:                      Number of iterations the RANSAC
//                                             algorithm should execute.
//
// float ransac_inlier_threshold:              The distance threshold for 
//                                             considering a point an inlier
//                                             after projecting it w/ the 
//                                             homography matrix.
//
// return: 3x3 matrix which best approximates the transformation from
//         good_matches_scene to good_matches_object
//==============================================================================
cv::Mat algorithms::calculateHomography(const std::vector<cv::Point2f>& good_matches_scene,
                        const std::vector<cv::Point2f>& good_matches_object,
                        int ransac_iterations,
                        float ransac_inlier_threshold,
                        std::vector<int> &inlier_indices)
{
    inlier_indices.clear();
    return cv::Mat::eye(3, 3, CV_32F);
}



//==============================================================================
//void warpImage(cv::Mat& img_object,
//                        cv::Mat& img_scene, cv::Mat& img_rep, 
//                        cv::Mat& warped_rep_img, cv::Mat& H,
//                        std::vector<Point2f> &scene_corners,
//                        cv::Mat& img_scene_lines )
//------------------------------------------------------------------------------
// Calculates the perspectiveTransform using the homography and warp the image
// correspondingly
//
// TODO:
//
// - get the 4 corners of the img_object
//
// - calculate the corresponding corners in the scene by using the OpenCV function 
// cv::perspectiveTransform and the homography. Insert the scene corners in the 
// output variable scene_corners.
//
// - Warp the the replacement image using the homography. Use the OpenCV function 
// cv::warpPerspective 
//
// Parameters:
//
// const cv::Mat& img_object: The Object image what should be replaced in the scene
//
// const cv::Mat& img_scene: The Scene image
//
// const cv::Mat& img_replacement: The replacement image for the img_object
//
// const cv::Mat& warped_replacement_img: The warped replacement image
//
// const vector<Point2f>& scene_corners: Corner points in the scene image that
//                                       correspond to the cornes of the 
//                                       img_object.
//============================================================
void algorithms::warpImage(const cv::Mat& img_object,
               const cv::Mat& img_scene, const cv::Mat& img_replacement, 
               cv::Mat& warped_replacement_img, cv::Mat& H,
               std::vector<cv::Point2f> &scene_corners)
{
    std::vector<cv::Point2f> object_corners(4);
    object_corners[0] = cv::Point2f(0, 0);
    object_corners[1] = cv::Point2f(static_cast<float>(img_object.cols), 0);
    object_corners[2] = cv::Point2f(static_cast<float>(img_object.cols), static_cast<float>(img_object.rows));
    object_corners[3] = cv::Point2f(0, static_cast<float>(img_object.rows));

    cv::perspectiveTransform(object_corners, scene_corners, H);

    cv::warpPerspective(img_replacement, warped_replacement_img, H, img_scene.size());

    cv::line(img_scene, scene_corners[0], scene_corners[1], cv::Scalar(0, 255, 0), 4);
    cv::line(img_scene, scene_corners[1], scene_corners[2], cv::Scalar(0, 255, 0), 4);
    cv::line(img_scene, scene_corners[2], scene_corners[3], cv::Scalar(0, 255, 0), 4);
    cv::line(img_scene, scene_corners[3], scene_corners[0], cv::Scalar(0, 255, 0), 4);
}

//==============================================================================
//void createMask(cv::Mat& img_object,
//                cv::Mat& img_scene, cv::Mat& masked_img_scene, 
//                cv::Mat& warped_rep_img, cv::Mat& final_image, 
//                std::vector<Point2f> &scene_corners,
//                cv::Mat& mask)
//------------------------------------------------------------------------------
//
//
// create a mask to add the warped replacement image on top of the scene image.
//
// Save the mask in the masked_image_scene.
//
// Combine the warped replacement image and the scene image. Use the mask image 
// to determine whether a pixel value from the warped replacement image or the 
// scene image should be used. Iterate through the mask image, for every pixel 
// value that is 0 take the pixel value from the warped replacement 
// image, otherwhise take the value from the scene image. Store the chosen pixel
//  values in the final image at the according position.
//
// TODO:
//
// - get the 4 corners of the img_object
//
// - calculate the corresponding corners in the scene by using the OpenCV function 
//   cv::perspectiveTransform and the homography. Insert the scene corners in the 
//   output variable scene_corners.
//
// - Warp the the replacement image using the homography. 
//   Use the OpenCV function cv::warpPerspective 
//
// Parameters:
//
// const cv::Mat& img_scene: The Scene image - use these pixels if the mask image 
//                       pixel is not set to 0.
//
// cv::Mat& mask: variable containing the resulting mask image.
//
// const cv::Mat& warped_rep_img: The warped replacement image - use these pixels if
//                            the mask image pixel is set to 0.
//
// cv::Mat& final_image: The final image - store the chosen pixel values
//                             at the according position here.
//
// const vector<Point2f>& scene_corners: Corner points in the scene image that
//                                       correspond to the cornes of the 
//                                       img_object.
//============================================================
void algorithms::createMask(const cv::Mat& img_scene, cv::Mat& mask, 
                cv::Mat& warped_rep_img, cv::Mat& final_image, 
                const std::vector<cv::Point2f> &scene_corners)
{
    mask = cv::Mat::zeros(img_scene.size(), CV_8UC1);
    final_image = img_scene.clone();

    std::vector<cv::Point> corners;
    for (const auto& pt : scene_corners) 
    {
        corners.push_back(cv::Point(static_cast<int>(pt.x), static_cast<int>(pt.y)));
    }

    cv::fillConvexPoly(mask, corners, cv::Scalar(255));

    for (int y = 0; y < mask.rows; ++y)
    {
        for (int x = 0; x < mask.cols; ++x) 
        {
            if (mask.at<uchar>(y, x) == 255) 
            {
                final_image.at<cv::Vec3b>(y, x) = warped_rep_img.at<cv::Vec3b>(y, x);
            }
        }
    }
}


// =============================== BONUS =======================================


//==============================================================================
// calcHarrisMeasure(const cv::Mat& img, cv::Mat& R)
//------------------------------------------------------------------------------
// Calculate the Harris Corner Measure
//
// TODO:
//
// - filter the image using a Gaussian kernel of size kernelSize
//
// - calculate the gradiants in x and y direction using a Sobel kernel
//
// - calculate Ixx, Ixy and Iyy using the formulas in the assignment sheet
//
// - filter Ixx, Ixy and Iyy with a Guassian kernal of size kernelSize
//
// - calculate the corner measure with the formula given in the assignment sheet
//   and save it in R
//
// - (hint: if done correctly no loops are needed)
//
// - note that the reference uses a regular float for the sigma, not a double
//
// Parameters:
//
// const cv::Mat& img: The input image
//
// cv::Mat& R: the corner measure for the image
//==============================================================================
void algorithms::calcHarrisMeasure(const cv::Mat& img, cv::Mat& R)
{
    float kappa = 0.05;
}

//==============================================================================
// Fast(const cv::Mat& img, std::vector<KeyPoint>& features, 
//      const vector<cv::Point> &circle, const cv::Mat& R, int N, int thresh, 
//      float harrisThreshold)
//------------------------------------------------------------------------------
// Find feature points using the FAST algorithm
//
// TODO:
//
// - calculate a lower and upper threshold
//
// - for every pixel check if the harris corner measure is higher than a given
//   threshold
//
// - if higher perform the FAST algorithm
//
// - check if N consecutive points are higher or lower than the threshold
//
// - at the same time calculate the sum of absolute differences (SAD) between the
//   center and the current pixel on the circle
//
// - if N consecutive pixels on the circle are higher/lower than the threshold 
//   mark this point as feature point
//
// - perform a non-maxima suppression: for every feature point that has an 
//   Euclidean distance less equal than 9 check if
//   its SAD is higher or lower than the current feature point, only keep the point
//   with the highest SAD
//
// - return the found feature points in features
//
//
// Parameters:
//
// const cv::Mat& img: the input image
//
// std::vector<KeyPoint>& features: the found feature points
//
// vector<cv::Point> circle: points on a circle with radius 6
//
// const cv::Mat& R: harris corner measure
//
// int N: number of consecutive points that should be higher/lower
//
// int thresh: value that should be used to calculate the upper and lower
//             threshold 
//
// float harrisThreshold: threshold for the harris corner measure
//==============================================================================
void algorithms::Fast(const cv::Mat& img, std::vector<cv::KeyPoint>& features, 
          const std::vector<cv::Point> &circle, const cv::Mat& R, int N, int thresh, 
          float harrisThreshold)
{
}


//==============================================================================
// Brief(const cv::Mat& img, const vector<BRIEF> &tests, const std::vector<KeyPoint>
// &features, cv::Mat& descriptors)
//------------------------------------------------------------------------------
// calculate the BRIEF descriptor
//
// - iterate over the feature points and perform a binary test with all test
//   given in the vector tests
//
// - the result of every binary test is stored in a bit of the feature
//   descriptor
//
// Parameters:
//
// const cv::Mat& img: the input image
//
// const vector<BRIEF> &tests: list of test points for the binary test
//
// const std::vector<KeyPoint> &features: the feature points found with FAST
//
// cv::Mat& descriptors: list of the descriptors
//==============================================================================
void algorithms::Brief(const cv::Mat& img, const std::vector<algorithms::BRIEF>& tests,
           const std::vector<cv::KeyPoint>& features, cv::Mat& descriptors) 
{
    const size_t byte_size = 8;
    const int bytes = tests.size() / 8;
}
