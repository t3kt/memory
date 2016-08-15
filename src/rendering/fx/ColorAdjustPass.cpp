//
//  ColorAdjustPass.cpp
//  memory
//
//  Created by tekt on 8/10/16.
//
//

#include <string>
#include "../rendering/fx/ColorAdjustPass.h"

#define _(A) #A

// see http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
const char* FRAGSRC=
_(
  uniform sampler2D tex;
  uniform vec3 offset;
  uniform vec3 mult;
  uniform float brightness;
  uniform float contrast;

  vec3 rgb2hsv(vec3 c)
  {
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
  }
  vec3 hsv2rgb(vec3 c)
  {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
  }

  vec3 applyHsvAdjust(vec3 color) {
    return hsv2rgb((rgb2hsv(color) + offset) * mult);
  }

  vec3 applyBrightContrast(vec3 color) {
    vec3 colorContrasted = (color) * contrast;
    return colorContrasted + vec3(brightness);
  }

  void main() {
    vec4 color = texture2D(tex, gl_TexCoord[0].st);
    color.rgb = applyHsvAdjust(color.rgb);
    color.rgb = applyBrightContrast(color.rgb);
    gl_FragColor = color;
  }
);

ColorAdjustPass::ColorAdjustPass(const ofVec2f& aspect,
                                 bool arb)
: itg::RenderPass(aspect, arb, "coloradjust") {
  _shader.setupShaderFromSource(GL_FRAGMENT_SHADER, FRAGSRC);
  _shader.linkProgram();
}

void ColorAdjustPass::render(ofFbo &readFbo,
                             ofFbo &writeFbo,
                             ofTexture &depth) {
  writeFbo.begin();
  _shader.begin();
  _shader.setUniformTexture("tex", readFbo.getTexture(), 0);
  _shader.setUniform3f("offset",
                       _params->hueOffset.get(),
                       0,
                       0);
  _shader.setUniform3f("mult",
                       1,
                       _params->saturationMult.get(),
                       _params->valueMult.get());
  _shader.setUniform1f("brightness",
                       _params->brightness.get());
  _shader.setUniform1f("contrast",
                       _params->contrast.get());

  texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());

  _shader.end();
  writeFbo.end();
}
