
#include <string>
#include <cstdio>
#include <chrono>
using namespace std::chrono_literals;

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/filesystem.hpp>

namespace vic {

void calculateNvdi( cv::InputArray iimg, cv::OutputArray oimg ) {
  cv::Mat img = iimg.getMat();

  cv::Mat nvdi( iimg.size(), CV_32FC1 );
  img.forEach<cv::Vec3b>([&](cv::Vec3b &ngb, const int *position) -> void {
    double num = ngb[2] - ngb[0];
    double den = ngb[2] + ngb[0];
    nvdi.at<float>( position ) = den != 0 ? num / den : 0.0;
  });
  
  oimg.assign( nvdi );
}

void colormapNvdi( cv::InputArray iimg, cv::OutputArray oimg ) {
  cv::Mat nvdi = iimg.getMat();

  cv::Mat img( iimg.size(), CV_8UC3 );
  img.forEach<cv::Vec3b>([&](cv::Vec3b &bgr, const int *position) -> void {
    float nvd = nvdi.at<float>( position );
    bgr[2] = (nvd < 0.0)  ? 0 : (nvd >= 0.5)  ? 255 : (uint8_t)(nvd / 0.5 * 255);
    bgr[1] = (nvd < -0.5) ? (uint8_t)((nvd + 1) / 0.5 * 255) : (nvd > 0.5) ? (uint8_t)(255 - ((nvd - 0.5) / 0.5 * 255)) : 255;
    bgr[0] = (nvd > 0.0)  ? 0 : (nvd <= -0.5) ? 255 : (uint8_t)(nvd * -1.0 / 0.5 * 255);
  });

  oimg.assign( img );
}

} // namespace vic

int main( int argc, char *argv[] ) {
  bool is_done = false;
  bool on_hold = false;
  std::vector<cv::String> filenames;
  cv::glob( "*.jpg", filenames );
  auto fileiter = filenames.begin();
  cv::Mat lastnvdi;
  cv::Rect baselineroi;

  if( filenames.empty() ) {
    printf( "Cannot find image files. Exitting...\n" );
    exit( 1 );
  }

  //cv::namedWindow("disp" );
  //cv::setMouseCallback();

  while( ! is_done ) {
    const auto &filename = *fileiter;
    cv::Mat img = cv::imread( filename );

    cv::Mat nvdi;
    vic::calculateNvdi( img, nvdi );
    cv::Mat nvdidiff = nvdi - lastnvdi;
    lastnvdi = nvdi;

    cv::Mat nvdinorm;
    if( !baselineroi.empty() ) {
      double val = cv::mean( cv::Mat( nvdi, baselineroi ) )[0];
      nvdinorm = (nvdi - val)*2;
    } else {
      cv::normalize( nvdi, nvdinorm, 1, 0, cv::NORM_MINMAX );
    }
    vic::colormapNvdi( nvdinorm, img );
    cv::imshow( "disp", img );

    cv::Mat diffimg;
    if( !baselineroi.empty() ) {
      nvdinorm = nvdidiff*2;
    } else {
      cv::normalize( nvdidiff, nvdinorm, 1, 0, cv::NORM_MINMAX );
    }
    vic::colormapNvdi( nvdinorm, diffimg );
    cv::imshow( "diff disp", diffimg );

    auto start = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> timeleft = 2s;
    do {
      int key = cv::waitKey( (int)timeleft.count() );
      switch( key ) {
      case 'Q': case 'q':
        is_done = true;
        break;
      case 'b':
        baselineroi = cv::selectROI( "disp", img );
        break;
      }
      timeleft = (2s - (std::chrono::system_clock::now() - start) );
    } while( timeleft > 0s );
    if( !on_hold ) {
      fileiter++;
      if( fileiter == filenames.end() ) {
        fileiter = filenames.begin();
      }
    }
  }

  exit( 0 );
}

