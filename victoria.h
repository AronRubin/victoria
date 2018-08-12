
// Use https://en.cppreference.com/w/cpp for anything you do not understand

/* C and C++ are unique among many languages as they have 4 stages to the
 * compilation process - preprocessor, compilation to assembly, compilation
 * to machine binary, and linking. Note we often think of compilation to
 * binary as including the compilation to assembly as that happens in a
 * single command. The preprocessor directives, beginning the line with a #,
 * are used to weave the different bits of code that will actually be
 * compiled into a single file, apply litteral values that are better
 * expressed as words (e.g. PI), and transform bits of code in-place for
 * efficiency or convenience (a.k.a. macros).
 */

/* This is called an include guard. It prevents the contents of the file
 * from being read more than once. It works in the same way the following
 * code would:
 * is_not_first = false;
 * if ( ! is_not_first ) {
 *   is_not_first = true;
 *   do_stuff();
 * }
 */
#ifndef VICTORIA_H_INCLUDED
#define VICTORIA_H_INCLUDED

// 
#include <opencv2/opencv.hpp>

// This is a namespace statement. It causes everything inside to have vic::
// prepended to their name
namespace vic {

/* Lets create an aggregate type. The following code is called a class
 * declaration. Unless code is placed inline, it is only a statement of the
 * existance of code. When the software is "linked" the code that references
 * (uses) the declarations will be matched with the actual definitions of the
 * code.
 */
class VictoriaApplication {
  /* "public" is an access specifier, which are followed by a colon.
   * Everything after an access specifier and before the next access specifier
   * can be used in the specified context. It is like your house. Public is
   * the common area outside of your house for all to see and touch. Protected
   * is inside your house where family and friends can see and use. Private is
   * only you.
   */
public:
  
  /* Classes in c++ have constructors. A default one will be made if you do
   * not make it yourself. Here we have the first example of a pointer
   * represented by an asterisk. Pointers hold the address in memory of
   * something of value with the preceding type. In this case argv is an
   * array of unknown length of addresses/pointers to characters, char.
   */
  VictoriaApplication( int argc, char *argv[] );

  ~VictoriaApplication();
}; // note the semicolon following the class declaration

// The following unnecessary structured code but would be more useful in a
// complex program.

class ImageFilter {
public:
  virtual void prepare();
  virtual bool filter( cv::InputArray in_image, cv::OutputArray out_image ) = 0;
};

class NvdiFilter : public ImageFilter {
  bool filter( cv::InputArray in_image, cv::OutputArray out_image ) override;
};

class ColormapFilter : public ImageFilter {
  bool filter( cv::InputArray in_image, cv::OutputArray out_image ) override;
};

} // namespace vic

#endif // VICTORIA_H_INCLUDED