#include "algorithms.h"

//===============================================================================
// compute_grayscale()
//-------------------------------------------------------------------------------
// TODO:  - iterate over all pixels in the image.
//        - multiply [R,G,B] with [0.2989, 0.5870, 0.1140] and accumulate.
// hints: - use cv::Vec3b type to access the color values of a 3-channel image.
//        - be aware that OpenCV treats matrix accesses in row-major order;
//          iterate through rows, then columns!
//
// parameters:
//  - input_image: [CV_8UC3] input image for the grayscale calculation
//  - grayscale_image: [CV_8UC1] output grayscale image
// return: void
//===============================================================================
void algorithms::compute_grayscale(const cv::Mat &input_image, cv::Mat &grayscale_image)
{
    cv::Vec3b rgb = 0;

    for(size_t row = 0; row < input_image.rows; ++row)
    {
        for(size_t column = 0; column < input_image.cols; ++column)
        {
            rgb =  input_image.at<cv::Vec3b>(row, column);

            grayscale_image.at<uchar>(row, column) = rgb[2] * 0.2989 + rgb[1] * 0.5870 + rgb[0] * 0.1140;

        }
    }
    
        
}

//===============================================================================
// compute_histogram()
//-------------------------------------------------------------------------------
// TODO:  - calculate the histogram of pixel intensities in the grayscale image.
//        - normalize the histogram values.
// hints: - Histogram is a 2D array which is initialized to zero, where each
//          index, i.e. bin, corresponds to a different pixel intensity.
//
// parameters:
//  - grayscale_image: [CV_8UC1] input grayscale image
//  - normalized_histogram: [Histogram] output normalized histogram data
// return: void
//===============================================================================
void algorithms::compute_histogram(const cv::Mat &grayscale_image, Histogram &normalized_histogram)
{
    for(size_t row = 0; row < grayscale_image.rows; row++)
    {
        for(size_t column = 0; column < grayscale_image.cols; column++)
        {
            auto temp = grayscale_image.at<uchar>(row, column);
            normalized_histogram[temp] += 1;
        }
    }

    auto size = grayscale_image.rows * grayscale_image.cols;

    for(size_t i = 0; i < 256; i++ )
    {
        normalized_histogram[i] = normalized_histogram[i] / size;
    }

}

//===============================================================================
// compute_threshold()
//-------------------------------------------------------------------------------
// TODO:  - implement OTSU's method (according to the assignment) sheet to
//          calculate an optimal threshold value for binary image segmentation.
//        - use the normalized histogram to determine the inter-class variance
//          for each possible threshold value t ∈ [1,255].
//        - find the optimal threshold value by maximizing the inter-class
//          variance and save it in 'threshold'.
// hints: - the inter-class variance is calculated using the probablities and the
//          mean intensities of each class (background, foreground).
//
// parameters:
//  - normalized_histogram: [Histogram] input normalized histogram data
//  - threshold: [int] output optimal threshold value
// return: void
//===============================================================================
void algorithms::compute_threshold(const Histogram &normalized_histogram, int &threshold)
{
    const int L = 256;  // bins, i.e., intensity levels

    auto current_best = 0.0;

    for(size_t tresh = 1; tresh < L; tresh++)
    {
        auto w0 = 0.0;
        auto w1 = 0.0;
        for (size_t i = 0; i < tresh;i++ )
        {
            w0 += normalized_histogram[i];
        }

        w1 = 1.0 - w0;

        auto mu0 = 0.0;
        for (size_t i = 0; i < tresh;i++ )
        {
            mu0 += i * normalized_histogram[i];
        }

        mu0 = mu0 / w0;


        auto mu1 = 0.0;
        for (size_t i = tresh; i < L;i++ )
        {
            mu1 += i * normalized_histogram[i];
        }

        mu1 = mu1 / w1;

        auto running_value  = w0 * w1 * pow((mu0 - mu1), 2);

        if(running_value > current_best)
        {
            current_best = running_value;
            threshold = tresh;
        }


    }
}

//===============================================================================
// compute_binary()
//-------------------------------------------------------------------------------
// TODO:  - apply the optimal threshold value to convert the grayscale image into
//          an inverted binary image.
//        - for pixel intensities above the threshold, the corresponding pixel in
//          the binary image should be set to black (0); otherwise white (255).
// hints: - compare each pixel value against the threshold to determine its new
//          color.
//
// parameters:
//  - grayscale_image: [CV_8UC1] input grayscale image
//  - threshold: [int] calculated optimal threshold value
//  - binary_image: [CV_8UC1] output binary image
// return: void
//===============================================================================
void algorithms::compute_binary(const cv::Mat &grayscale_image, const int threshold, cv::Mat &binary_image)
{
    for(size_t row = 0; row < grayscale_image.rows; row++)
    {
        for(size_t column = 0; column < grayscale_image.cols; column++)
        {
            uchar intesity = grayscale_image.at<uchar>(row, column);
            if (intesity > threshold)
            {
                binary_image.at<uchar>(row, column) = 0;
            }
            else
            {
                binary_image.at<uchar>(row, column) = 255;
            }

        }
    }

}

//===============================================================================
// draw_contours()
//-------------------------------------------------------------------------------
// TODO:  - find contours in the binary image using cv::findContours(); use mode
//          cv::RETR_EXTERNAL to obtain outer contours only.
//        - draw all identified contours using cv::drawContours(); use the
//          cv::FILLED parameter to fill enclosed areas.
//        - save the filled image and contours to 'filled_image' and 'contours',
//          respectively.
// hints: - choose the fill color such that the enclosed area is white (255).
//
// parameters:
//  - binary_image: [CV_8UC1] input binary image
//  - filled_image: [CV_8UC1] output image with filled area
//  - contours: [std::vector<std::vector<cv::Point>>] output 2D vector to store
//              all detected contours
// return: void
//===============================================================================
void algorithms::draw_contours(const cv::Mat &binary_image, cv::Mat &filled_image,
                               std::vector<std::vector<cv::Point>> &contours)
{
    cv::findContours(binary_image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE );

    cv::drawContours(filled_image, contours, -1, cv::Scalar(255), cv::FILLED);
}

//===============================================================================
// apply_morph_operation()
//-------------------------------------------------------------------------------
// TODO:  - apply a morphological operation (either erosion or dilation) to the
//          filled image using the specified kernel size and mode.
//        - implement the logic behind erosion and dilation according to the
//          assignment sheet.
//        - store the result in 'morphed_image'.
// hints: - the kernel size represents the area around each pixel in the image
//          considered during the morpholocial operation; the kernel size
//          is given as an odd number to ensure proper centering.
//
// parameters:
//  - filled_image: [CV_8UC1] noisy output image with filled area
//  - kernel_size: [int] size of the (square-shaped) kernel
//  - mode: [cv::MorphTypes] type of morphological operation; this will either
//          be MORPH_ERODE or MORPH_DILATE
//  - morphed_image: [CV_8UC1] output image after applied morphological operation;
//                   either dilated or eroded image, depending on mode
// return: void
//===============================================================================
void algorithms::apply_morph_operation(const cv::Mat &filled_image, const int kernel_size, const cv::MorphTypes mode,
                                       cv::Mat &morphed_image)
{


    for(size_t row = 0; row < filled_image.rows; row++)
    {
        for(size_t column = 0; column < filled_image.cols; column++)
        {
            //find up - and leftmost point of matrix
            if( filled_image.at<uchar>(row, column) == 0 && mode == cv::MORPH_ERODE) //only white for erosion
            {
                morphed_image.at<uchar>(row, column) = filled_image.at<uchar> (row, column);
                continue;
            }

            if( filled_image.at<uchar>(row, column) > 0 && mode == cv::MORPH_DILATE)//only black for dilation
            {
                morphed_image.at<uchar>(row, column) = filled_image.at<uchar> (row, column);
                continue;
            }



            auto offset = (kernel_size -1) / 2;

            uchar pixel_intesity = filled_image.at<uchar> (row, column);
            
            std::pair<size_t, size_t> kernel_start;
            kernel_start.first = row - offset;
            kernel_start.second = column - offset;

            // uchar best = pixel_intesity;
            uchar best = 0;
            if (mode == cv::MORPH_ERODE)
            {
                best = 255;
            }


            for(size_t k_row = 0; k_row < kernel_size; k_row++)
            {
                for(size_t k_col = 0; k_col < kernel_size; k_col++)
                {
                    //evaluate brightness
                    //save brightest or darkest value

                    if(kernel_start.first + k_row >= 0 && kernel_start.first + k_row < filled_image.rows
                    && kernel_start.second + k_col >= 0 && kernel_start.second + k_col < filled_image.cols)
                    {
                        uchar brightness = filled_image.at<uchar>(kernel_start.first + k_row, kernel_start.second + k_col);

                        if (brightness < best && mode == cv::MORPH_ERODE )
                        {
                            best = brightness;
                        }

                        else if (brightness > best && mode == cv::MORPH_DILATE)
                        {
                            best = brightness;
                        }
                    }
                }
            }
            morphed_image.at<uchar>(row, column) = best;
            //set anchor to brightest value and saved in morphed
        }
    }


}

//===============================================================================
// calc_area()
//-------------------------------------------------------------------------------
// TODO:  - calculate the area of the filled region as the sum of pixels in the
//          normalized image.
//        - convert the resulting area from pixel units to real-world units using
//          the provided conversion factor.
//        - save the calculated areas to 'area_px' and 'area_km2'.
// hints: - the final area must be stored in square kilometers; convert your
//          result accordingly.
//
// parameters:
//  - normalized_image: [CV_8UC1] input normalized image with filled area
//  - conversion_factor_m2: [float] conversion factor from pixels to square meters;
//                          this considers the scale of the image
//  - area_px: [float] output area in pixels
//  - area_km2: [float] output area in square kilometers
// return: void
//===============================================================================
void algorithms::calc_area(const cv::Mat &normalized_image, const float conversion_factor_m2, int &area_px,
                           float &area_km2)
{
    for(size_t row = 0; row < normalized_image.rows; row++)
    {
        for(size_t column = 0; column < normalized_image.cols; column ++)
        {
            if(normalized_image.at<uchar>(row, column))
            {
                area_px ++;
            }
        }
    }

    area_km2 = (area_px * conversion_factor_m2) /1000000;
}

//===============================================================================
// calc_perimeter()
//-------------------------------------------------------------------------------
// TODO:  - calculate the perimeter of the largest detected contour in the input
//          vector.
//        - For each pixel in the contour, count the outer edges by checking its
//          4-neighborhood; the total perimeter is the sum of outer edges.
//        - convert the resulting perimeter from pixel units to real-world units
//          using the provided conversion factor.
//        - save the calculated perimeters to 'perimeter_px' and 'perimeter_km'.
// hints: - find the largest contour in the given 2D vector of contours.
//        - utilize cv::pointPolygonTest() to check if neighboring pixels, i.e.,
//          edges of the pixel, lie outside the contour.
//
// parameters:
//  - contours: [std::vector<std::vector<cv::Point>>] input 2D vector with all
//              detected contours
//  - conversion_factor_m: [float] conversion factor from pixels to meters;
//                         this considers the scale of the image
//  - perimeter_px: [float] output perimeter in pixels
//  - perimeter_km: [float] output perimeter in kilometers
// return: void
//===============================================================================
void algorithms::calc_perimeter(const std::vector<std::vector<cv::Point>> &contours, const float &conversion_factor_m,
                                int &perimeter_px, float &perimeter_km)
{
    auto max_contour = contours[0].size();
    size_t chosen_index = 0;
    size_t i = 0;
    for (auto con: contours)
    {
        if(con.size() > max_contour)
        {
            max_contour = con.size();
            chosen_index = i;
        }
        i++;
    }


    for(auto edge: contours[chosen_index])
    {
        if(cv::pointPolygonTest(contours[chosen_index], edge + cv::Point(0,1), false) == -1)
        {
            perimeter_px++;
        }

        if(cv::pointPolygonTest(contours[chosen_index], edge + cv::Point(1,0), false) == -1)
        {
            perimeter_px++;
        }

        if(cv::pointPolygonTest(contours[chosen_index], edge + cv::Point(0,-1), false) == -1)
        {
            perimeter_px++;
        }

        if(cv::pointPolygonTest(contours[chosen_index], edge + cv::Point(-1,0), false) == -1)
        {
            perimeter_px++;
        }
    }

    perimeter_km = (perimeter_px * conversion_factor_m) /1000;
}


//===============================================================================
// calc_center_of_mass()
//-------------------------------------------------------------------------------
// TODO:  - calculate the center of mass for the filled region using pixel
//          intensities in the normalized input image.
//        - the center of mass is computed as the weighted average of the
//          spatial coordinates.
//        - save the center of mass in 'center_of_mass'.
// hints: - utilize the provided area in pixels to compute the center of mass.
//
// parameters:
//  - normalized_image: [CV_8UC1] input normalized image with filled area
//  - area_px: [float] area in pixels
//  - center_of_mass: [cv::Point] output center of mass
// return: void
//===============================================================================
void algorithms::calc_center_of_mass(const cv::Mat &normalized_image, const int area_px, cv::Point &center_of_mass)
{
    size_t upper = 0;

    for(size_t it = 0; it < normalized_image.rows; it++ )
    {
        for(size_t it2 = 0; it2 < normalized_image.cols; it2++)
        {
            upper += it2 * normalized_image.at<uchar>(it, it2);
        }
    }

    center_of_mass.x = upper / area_px;

    size_t  y_upper = 0;

    for(size_t it = 0; it < normalized_image.rows; it++ )
    {
        for(size_t it2 = 0; it2 < normalized_image.cols; it2++)
        {
            y_upper += it * normalized_image.at<uchar>(it, it2);
        }
    }

    center_of_mass.y = y_upper / area_px;

}

//===============================================================================
// crop_center_of_mass()
//-------------------------------------------------------------------------------
// TODO:  - convert the 1-channel *grayscale* image to a 3-channel image.
//        - use a circular mask, centered at the calculated center of mass, for
//          cropping; the size is given as 'crop_radius'.
//        - copy or replace the masked circular area in the grayscale image with
//          the *colored* input image.
//        - highlight the masked circular region by drawing a red circle around
//          the center.
//        - crop the final image to the correct size, specified by 'crop_size'.
//        - store the final image in 'cropped_center'.
// hints: - utilize cv::cvtColor() to convert the grayscale image; remember that
//          OpenCV uses BGR format.
//
// parameters:
//  - input_image: [CV_8UC3] input color image
//  - grayscale_image: [CV_8UC1] input grayscale image
//  - center_of_mass: [cv::Point] center of mass
//  - cropped_center: [CV_8UC3] cropped output image; highlighted center of mass
// return: void
//===============================================================================
void algorithms::crop_center_of_mass(const cv::Mat &input_image, const cv::Mat &grayscale_image,
                                     const cv::Point &center_of_mass, cv::Mat &cropped_center)
{
    const int crop_radius = 50;
    const cv::Size crop_size = {200, 200};
    const cv::Scalar crop_color = {0, 0, 255};

    cv::cvtColor(grayscale_image,cropped_center, cv::COLOR_GRAY2BGR);

    cv::Mat mask = cv::Mat::zeros(grayscale_image.size(), CV_8UC1);
    cv::circle(mask, center_of_mass, crop_radius, cv::Scalar(255), cv::FILLED);

    input_image.copyTo(cropped_center, mask);

    cv::circle(cropped_center,center_of_mass, crop_radius, crop_color);

    int crop_x = center_of_mass.x - crop_size.width / 2;
    int crop_y = center_of_mass.y - crop_size.height / 2;

    cv::Rect crop_rect(crop_x, crop_y, crop_size.width, crop_size.height);
    cropped_center = cropped_center(crop_rect);
    cv::resize(cropped_center, cropped_center, crop_size);
    
}

//===============================================================================
// bonus_find_contours()
//-------------------------------------------------------------------------------
// TODO:  - optional BONUS task
//        - implement the simplified contour tracing algorithm by Suzuki and Abe
//          to find contours in the binary image.
//        - save all detected contours as vectors of 2D points in 'contours'.
// hints: - follow the assignment sheet closely.
//
// parameters:
//  - binary_image: [CV_32SC1] input normalized binary image
//  - contours: [std::vector<std::vector<cv::Point>>] output 2D vector to store
//              all detected contours
// return: void
//===============================================================================
void algorithms::bonus_find_contours(cv::Mat &binary_image, std::vector<std::vector<cv::Point>> &contours)
{
    std::vector<cv::Point> neighbour_offset = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    const int nr_neighbours = 4;
    int LNBD = 0;

}
