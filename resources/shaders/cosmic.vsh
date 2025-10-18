attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;
varying mediump vec2 v_screenTexCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
varying vec2 v_screenTexCoord;
#endif

uniform vec2 u_screenSize;

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;

    vec4 pos = CC_MVPMatrix * a_position;
    v_screenTexCoord = (pos.xy / pos.w) * 0.5 + 0.5;

    float aspect = u_screenSize.x / u_screenSize.y;
    v_screenTexCoord.x *= aspect;
}

