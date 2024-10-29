#version 330 core

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0; // La textura a aplicar
uniform float time;          // Tiempo desde el inicio
uniform float blinkFrequency; // Frecuencia del parpadeo
uniform float blinkAmplitude;  // Amplitud del parpadeo

void main()
{
    vec4 texColor = texture(texture0, fragTexCoord);
    
    // Controlar la opacidad con un efecto de parpadeo
    float alpha = (sin(time * blinkFrequency) * 0.5 + 0.5) * blinkAmplitude; // Escalar entre 0 y blinkAmplitude
    finalColor = vec4(texColor.rgb, texColor.a * alpha); // Aplicar el parpadeo
}
