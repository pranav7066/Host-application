#include<string>
#include<stdio.h>
#include<stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "jpeg_decoder.h"

using namespace std;
using namespace cv;

Mat resize(string path_to_input){
  Mat img = imread(path_to_input, IMREAD_COLOR);
  const string out_image = "out/resized.jpg";
  if(img.empty())
  {
    cout << "Could not read the image: " << path_to_input << std::endl;
    exit(1);
  }
  int out_w = 416;
  int out_h = 416;
  Mat resizedImage;
  resize(img, resizedImage, Size(out_w, out_h));
  Size s = resizedImage.size();
  printf("Image resized with height: %d, width: %d\n", s.height, s.width);
  imwrite(out_image, resizedImage);
  return resizedImage;
}

void decode(const char* path){
  size_t size;
  unsigned char *buf;
  FILE *f;
  f = fopen(path, "rb");
  fseek(f, 0, SEEK_END);
  size = ftell(f);
  buf = (unsigned char*)malloc(size);
  fseek(f, 0, SEEK_SET);
  size_t read = fread(buf, 1, size, f);
  fclose(f);
  Jpeg::Decoder decoder(buf, size);
  if (decoder.GetResult() != Jpeg::Decoder::OK){
    printf("Error decoding the input file\n");
  }
  f = fopen("out/decoded+resized.bin", "wb");
  fwrite(decoder.GetImage(), 1, decoder.GetImageSize(), f);
  fclose(f);
  printf("Done decoding...");
}

int model_to_bin(const char* srcfilename, const char* dstfilename)
{
  long  len;
  char* buf = NULL;
  FILE* fp  = NULL;
  fp = fopen(srcfilename, "rb");
  if (!fp) return 0;

  if (fseek(fp, 0, SEEK_END) != 0)
  {
    fclose(fp);
    return 0;
  }
  len = ftell(fp);
  rewind(fp);
  buf = (char*)malloc(len);
  if (!buf)
  {
    fclose(fp);
    return 0;
  }
  if (!fread(buf, len, 1, fp))
  {
    free(buf);
    fclose(fp);
    return 0;
  }
  fclose(fp);
  fp = fopen( dstfilename, "wb" );
  if (!fp)
  {
    free(buf);
    return 0;
  }
  if (!fwrite(buf, len, 1, fp))
  {
    free(buf);
    fclose(fp);
    return 0;
  }
  fclose(fp);
  free(buf);
  return 1;
  printf("Model written to bin...");
}
