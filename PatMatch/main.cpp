//
//  main.cpp
//  PatMatch
//
//  Created by Liu Wei on 9/27/16.
//  Copyright © 2016 Liu Wei. All rights reserved.
//

#include <iostream>
#include "lodepng.h"
#include <string>

#include <boost/program_options.hpp>
using namespace boost::program_options;
using namespace std;

int main(int argc, const char * argv[]) {
    
    int e = 10;

    options_description desc("Allowed options");
    desc.add_options()
    ("tolerance,t", value(&e)->default_value(10), "matching tolerance")
    ("pattern,p", value<std::string>()->required(), "pattern file path")
    ;
    
    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
//    notify(vm);
    
    e = vm["tolerance"].as<int>();
    
    std::vector<unsigned char> screen; //the raw pixels
    unsigned w, h;
    
    //decode
    unsigned error = lodepng::decode(screen, w, h, "/Users/wei/sandbox/gamebot/screen.png");
    
    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    std::vector<unsigned char> pat; //the raw pixels
    unsigned pw, ph;
    error = lodepng::decode(pat, pw, ph, vm["pattern"].as<string>());

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    
    const int ERR = pw*ph*3 * e;
    
    for(int x=0; x<w-pw; x++) {
        for(int y=0; y<h-ph; y++) {
            int err = 0;
            for(int px=0; px<pw; px++) {
                for(int py=0; py<ph; py++){
                    for(int c=0; c<3; c++) {
                        err += abs(screen[((y+py)*w + x+px)*4 + c] - pat[(py*pw+px)*4 + c]);
                        if(err>ERR) break;
                    }
                }
            }
            
            if(err<=ERR) {
                printf("%d %d\n", x+pw/2, y+ph/2);
               // fprintf(stderr, "err = %d\n", err);

               return 0;
            }
        }
    }
    
    return 1;
}
