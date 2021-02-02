//
//  ViewController.m
//  small3dios
//
//  Created by me on 17/09/2019.
//  Copyright © 2019 dimi309. All rights reserved.
//

#import "ViewController.h"
#include <iostream>
#include <sys/time.h>
#include "interop.h"
#include "flags.h"
#include "KeyInput.hpp"
#include "GameLogic.hpp"

using namespace AvoidTheBug3D;
using namespace std;

GameLogic *gameLogic;
KeyInput input;

float halfx = 0.0f;
float halfy = 0.0f;

double currentTimeInSeconds()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec / 1000000;
}

void touched(int x, int y) {
  if (x < halfx) {
    input.left = true;
    input.right = false;
  }
  else {
    input.left = false;
    input.right = true;
  }
  
  if (y < halfy) {
    input.up = true;
    input.down = false;
  }
  else {
    input.up = false;
    input.down = true;
  }
}

@implementation ViewController {
  CADisplayLink* _displayLink;
}

- (void)viewDidLoad {
  [super viewDidLoad];
  
  
  // Do any additional setup after loading the view.
  app_window = (__bridge void*) self.view.layer;
  
  gameLogic = new GameLogic();
  
  halfx = gameLogic->renderer->getScreenWidth() / 2;
  halfy = gameLogic->renderer->getScreenHeight() / 2;
  
  uint32_t fps = 60;
  _displayLink = [CADisplayLink displayLinkWithTarget: self selector: @selector(renderLoop)];
  [_displayLink setFrameInterval: 60 / fps];
  [_displayLink addToRunLoop: NSRunLoop.currentRunLoop forMode: NSDefaultRunLoopMode];
  
}

- (void) viewDidAppear:(BOOL)animated {
  
}

- (void) viewWillDisappear:(BOOL)animated {
  
}

-(void) renderLoop {
  if (app_active) {
    gameLogic->process(input);
    gameLogic->render();
  }
}



- (void)touchesBegan:(NSSet<UITouch *> *)touches
           withEvent:(UIEvent *)event {
  UITouch *touch = [[event allTouches] anyObject];
  CGPoint touchPoint = [touch locationInView:self.view];
  input.enter = true;
  touched(touchPoint.x, touchPoint.y);
  //LOGDEBUG("touch begin " + intToStr(touchPoint.x) + " - " + intToStr(touchPoint.y));
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches
           withEvent:(UIEvent *)event {
  UITouch *touch = [[event allTouches] anyObject];
  CGPoint touchPoint = [touch locationInView:self.view];
  input.enter = true;
  touched(touchPoint.x, touchPoint.y);
  //LOGDEBUG("touch move " + intToStr(touchPoint.x) + " - " + intToStr(touchPoint.y));
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches
           withEvent:(UIEvent *)event {
  input = {};
}


@end

@implementation View
+(Class) layerClass { return [CAMetalLayer class]; }
@end
