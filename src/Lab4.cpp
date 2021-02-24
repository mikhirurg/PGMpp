//
// Created by @mikhirurg on 06.06.2020.
//

#include <iostream>
#include "PGMImage.h"
#include "PGMSpace.h"

struct globArgs {
  std::string from;
  std::string to;
  int in_count;
  std::string in_name;
  int out_count;
  std::string out_name;
} glob_args;

void log() {
  std::cout << "from_color_space: " << glob_args.from << std::endl;
  std::cout << "to_color_space: " << glob_args.to << std::endl;
  std::cout << "input_count " << glob_args.in_count << std::endl;
  std::cout << "input_name " << glob_args.in_name << std::endl;
  std::cout << "output_count " << glob_args.out_count << std::endl;
  std::cout << "output_name " << glob_args.out_name << std::endl;
}

int main(int argc, char *argv[]) {
  std::set<std::string> valid_spaces = {
      "RGB", "HSL", "HSV", "YCbCr.601", "YCbCr.709", "YCoCg", "CMY"
  };
  try {
    if (argc != 11) {
      throw PGMImageParamsException();
    }
    std::vector<std::string> opts;
    for (int i = 1; i < argc; i++) {
      opts.emplace_back(argv[i]);
    }
    while (!opts.empty()) {
      if (opts[0] == "-f") {
        if (opts.size() > 1 && valid_spaces.find(opts[1]) != valid_spaces.end()) {
          glob_args.from = opts[1];
        } else {
          throw PGMImageParamsException();
        }
        opts.erase(opts.begin());
        opts.erase(opts.begin());
        continue;
      }
      if (opts[0] == "-t") {
        if (opts.size() > 1 && valid_spaces.find(opts[1]) != valid_spaces.end()) {
          glob_args.to = opts[1];
        } else {
          throw PGMImageParamsException();
        }
        opts.erase(opts.begin());
        opts.erase(opts.begin());
        continue;
      }
      if (opts[0] == "-i") {
        if (opts.size() <= 2) {
          throw PGMImageParamsException();
        }
        try {
          glob_args.in_count = std::stoi(opts[1]);
          glob_args.in_name = opts[2];
        } catch (std::bad_cast &) {
          throw PGMImageParamsException();
        }
        if (!(glob_args.in_count == 1 || glob_args.in_count == 3)) {
          throw PGMImageParamsException();
        }
        opts.erase(opts.begin());
        opts.erase(opts.begin());
        opts.erase(opts.begin());
        continue;
      }
      if (opts[0] == "-o") {
        if (opts.size() <= 2) {
          throw PGMImageParamsException();
        }
        try {
          glob_args.out_count = std::stoi(opts[1]);
          glob_args.out_name = opts[2];
        } catch (std::bad_cast &) {
          throw PGMImageParamsException();
        }
        if (!(glob_args.out_count == 1 || glob_args.out_count == 3)) {
          throw PGMImageParamsException();
        }
        opts.erase(opts.begin());
        opts.erase(opts.begin());
        opts.erase(opts.begin());
      }
    }

    // log();

    int in_dot = glob_args.in_name.find_last_of('.');
    int out_dot = glob_args.out_name.find_last_of('.');
    if (in_dot == -1 || out_dot == -1) {
      throw PGMImageParamsException();
    }
    std::string input_name = glob_args.in_name.substr(0, in_dot);
    std::string input_ext = glob_args.in_name.substr(in_dot, glob_args.in_name.length());
    std::string output_name = glob_args.out_name.substr(0, out_dot);
    std::string output_ext = glob_args.out_name.substr(out_dot, glob_args.out_name.length());

    PGMImage<PGMColorPixel> *p_img;

    if (glob_args.in_count == 3) {
      PGMImage<PGMMonoPixel> img1 = PGMImage<PGMMonoPixel>(input_name+"_1"+input_ext, 1);
      PGMImage<PGMMonoPixel> img2 = PGMImage<PGMMonoPixel>(input_name+"_2"+input_ext, 1);
      PGMImage<PGMMonoPixel> img3 = PGMImage<PGMMonoPixel>(input_name+"_3"+input_ext, 1);
      p_img = new PGMImage<PGMColorPixel>(img1, img2, img3);
    } else {
      p_img = new PGMImage<PGMColorPixel>(glob_args.in_name, 1);
    }

    PGMImage<PGMColorPixel> tmp = PGMImage<PGMColorPixel>(p_img->GetWidth(),
                                                  p_img->GetHeight(),
                                                  p_img->GetMaxVal(),
                                                  p_img->GetFileType(),
                                                  p_img->GetGamma());
    if (glob_args.from == std::string("RGB") && glob_args.to == std::string("RGB")) {
      for (int y = 0; y < p_img->GetHeight(); y++) {
        for (int x = 0; x < p_img->GetWidth(); x++) {
          tmp.PutPixel(x, y, p_img->GetPixel(x, y));
        }
      }
    } else {
      if (glob_args.from == std::string("RGB")) {
        PGMSpace &color_space = PGMSpace::CSpaceByName(glob_args.to);
        for (int y = 0; y < p_img->GetHeight(); y++) {
          for (int x = 0; x < p_img->GetWidth(); x++) {
            tmp.PutPixel(x, y, color_space.FromRGB(p_img->GetPixel(x, y)));
          }
        }
      } else if (glob_args.to == std::string("RGB")) {
        PGMSpace &color_space = PGMSpace::CSpaceByName(glob_args.from);
        for (int y = 0; y < p_img->GetHeight(); y++) {
          for (int x = 0; x < p_img->GetWidth(); x++) {
            tmp.PutPixel(x, y, color_space.ToRGB(p_img->GetPixel(x, y)));
          }
        }
      } else {
        PGMSpace &color_space_from = PGMSpace::CSpaceByName(glob_args.from);
        PGMSpace &color_space_to = PGMSpace::CSpaceByName(glob_args.to);
        for (int y = 0; y < p_img->GetHeight(); y++) {
          for (int x = 0; x < p_img->GetWidth(); x++) {
            tmp.PutPixel(x, y, color_space_to.FromRGB(color_space_from.ToRGB(p_img->GetPixel(x, y))));
          }
        }
      }
    }

    if (glob_args.out_count == 3) {
      auto R = tmp.GetImageByChannel('R');
      auto G = tmp.GetImageByChannel('G');
      auto B = tmp.GetImageByChannel('B');
      R->WriteImg(output_name+"_1"+output_ext);
      G->WriteImg(output_name+"_2"+output_ext);
      B->WriteImg(output_name+"_3"+output_ext);
      delete R;
      delete G;
      delete B;
    } else {
      tmp.WriteImg(glob_args.out_name);
    }

    delete (p_img);
  } catch (PGMImageException e) {
    std::cerr << e.getErr();
    return 1;
  }
  return 0;
}