#version 330

// from VS
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

void main()
{
    if (fragColor.a != 1) {
        // Texel color fetching from texture sampler
        // NOTE: Calculate alpha using signed distance field (SDF)
        float distanceFromOutline = texture(texture0, fragTexCoord).a - 0.5;
        float distanceChangePerFragment = length(vec2(dFdx(distanceFromOutline), dFdy(distanceFromOutline)));
        float alpha = smoothstep(-distanceChangePerFragment, distanceChangePerFragment, distanceFromOutline);

        // Calculate final fragment color
        finalColor = vec4(fragColor.rgb, fragColor.a * alpha);
    } else {
        float sdf = (1 - texture(texture0, fragTexCoord).a) - 0.5;
        // sdf = 1 - ceil(sdf);
        // sdf = 1 - smoothstep(0, 0.2, sdf);
        // sdf = 1 - (sdf / 0.1);
        // sdf = (sdf * -1) / 0.1;

        float ddx = abs(dFdx(sdf));
        float ddy = abs(dFdy(sdf));
        float d = max(ddx, ddy);

        // sdf = (sdf * -1) / (ddx + ddy);
        sdf = ((sdf * -1) + (d * 0.5)) / d;

        finalColor = vec4(fragColor.rgb, sdf);
    }
}
