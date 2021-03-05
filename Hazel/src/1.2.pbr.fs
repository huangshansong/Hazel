#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform sampler2D texture_albedo[5];
uniform sampler2D texture_normal[5];
uniform sampler2D texture_metallic[5];
uniform sampler2D texture_roughness[5];
uniform sampler2D texture_ao[5];
uniform sampler2D texture_displacement[5];

// lights
uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform float heightScale;

uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir, int index)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(texture_displacement[index], currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(texture_displacement[index], currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(texture_displacement[index], prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}
// ----------------------------------------------------------------------------
void main()
{	
    // ----------------------------------------------------------------------------
    // Easy trick to get tangent-normals to world-space to keep PBR code simplified.
    // Don't worry if you don't get what's going on; you generally want to do normal 
    // mapping the usual way for performance anways; I do plan make a note of this 
    // technique somewhere later in the normal mapping tutorial.
    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);
    vec3 tN   = normalize(Normal);
    vec3 tT  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 tB  = -normalize(cross(tN, tT));
    mat3 TBN = mat3(tT, tB, tN);

    vec3 color[5];
    vec3 finalColor = vec3(0.0);
    
    if(WorldPos.y < 10.0)
    {
        // ----------------------------------------------------------------------------
        for(int i = 3; i < 4; i++)
        {
            vec3 tangentNormal = texture(texture_normal[i], TexCoords).xyz * 2.0 - 1.0;
            vec3 N = normalize(TBN * tangentNormal);
            vec3 V = normalize(camPos - WorldPos);

            vec3 tangentViewDir = TBN * V;
            vec2 texCoords = ParallaxMapping(TexCoords,  tangentViewDir, i);  
            
            vec3 albedo     = pow(texture(texture_albedo[i], texCoords).rgb, vec3(2.2));
            float metallic  = texture(texture_metallic[i], texCoords).r;
            float roughness = texture(texture_roughness[i], texCoords).r;
            float ao        = texture(texture_ao[i], texCoords).r;


            // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
            // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
            vec3 F0 = vec3(0.04); 
            F0 = mix(F0, albedo, metallic);

            // reflectance equation
            vec3 Lo = vec3(0.0);

            // calculate per-light radiance
            vec3 L = normalize(lightDirection);
            vec3 H = normalize(V + L);
            vec3 radiance = lightColor;

            // Cook-Torrance BRDF
            float NDF = DistributionGGX(N, H, roughness);   
            float G   = GeometrySmith(N, V, L, roughness);      
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
               
            vec3 nominator    = NDF * G * F; 
            float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
            vec3 specular = nominator / denominator;
            
            // kS is equal to Fresnel
            vec3 kS = F;
            // for energy conservation, the diffuse and specular light can't
            // be above 1.0 (unless the surface emits light); to preserve this
            // relationship the diffuse component (kD) should equal 1.0 - kS.
            vec3 kD = vec3(1.0) - kS;
            // multiply kD by the inverse metalness such that only non-metals 
            // have diffuse lighting, or a linear blend if partly metal (pure metals
            // have no diffuse light).
            kD *= 1.0 - metallic;	  

            // scale light by NdotL
            float NdotL = max(dot(N, L), 0.0);        

            // add to outgoing radiance Lo
            Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again   
            
            // ambient lighting (note that the next IBL tutorial will replace 
            // this ambient lighting with environment lighting).
            vec3 ambient = vec3(1) * albedo * ao;
            
            color[i] = ambient + Lo;

        }
        finalColor += color[3];   

    }else if(10.0 - 1E-5F < WorldPos.y && WorldPos.y < 11.0 + 1E-5F)
    {
        // ----------------------------------------------------------------------------
        for(int i = 3; i < 5; i++)
        {
            vec3 tangentNormal = texture(texture_normal[i], TexCoords).xyz * 2.0 - 1.0;
            vec3 N = normalize(TBN * tangentNormal);
            vec3 V = normalize(camPos - WorldPos);

            vec3 tangentViewDir = TBN * V;
            vec2 texCoords = ParallaxMapping(TexCoords,  tangentViewDir, i);  
            
            vec3 albedo     = pow(texture(texture_albedo[i], texCoords).rgb, vec3(2.2));
            float metallic  = texture(texture_metallic[i], texCoords).r;
            float roughness = texture(texture_roughness[i], texCoords).r;
            float ao        = texture(texture_ao[i], texCoords).r;


            // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
            // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
            vec3 F0 = vec3(0.04); 
            F0 = mix(F0, albedo, metallic);

            // reflectance equation
            vec3 Lo = vec3(0.0);

            // calculate per-light radiance
            vec3 L = normalize(lightDirection);
            vec3 H = normalize(V + L);
            vec3 radiance = lightColor;

            // Cook-Torrance BRDF
            float NDF = DistributionGGX(N, H, roughness);   
            float G   = GeometrySmith(N, V, L, roughness);      
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
               
            vec3 nominator    = NDF * G * F; 
            float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
            vec3 specular = nominator / denominator;
            
            // kS is equal to Fresnel
            vec3 kS = F;
            // for energy conservation, the diffuse and specular light can't
            // be above 1.0 (unless the surface emits light); to preserve this
            // relationship the diffuse component (kD) should equal 1.0 - kS.
            vec3 kD = vec3(1.0) - kS;
            // multiply kD by the inverse metalness such that only non-metals 
            // have diffuse lighting, or a linear blend if partly metal (pure metals
            // have no diffuse light).
            kD *= 1.0 - metallic;	  

            // scale light by NdotL
            float NdotL = max(dot(N, L), 0.0);        

            // add to outgoing radiance Lo
            Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again   
            
            // ambient lighting (note that the next IBL tutorial will replace 
            // this ambient lighting with environment lighting).
            vec3 ambient = vec3(1) * albedo * ao;
            
            color[i] = ambient + Lo;

        }
        float weight = (11.0 - WorldPos.y) / (11.0 - 10.0);
        finalColor = color[3] * weight + color[4] * (1.0 - weight);
    }else
    {
        // ----------------------------------------------------------------------------
        for(int i = 4; i < 5; i++)
        {
            vec3 tangentNormal = texture(texture_normal[i], TexCoords).xyz * 2.0 - 1.0;
            vec3 N = normalize(TBN * tangentNormal);
            vec3 V = normalize(camPos - WorldPos);

            vec3 tangentViewDir = TBN * V;
            vec2 texCoords = ParallaxMapping(TexCoords,  tangentViewDir, i);  
            
            vec3 albedo     = pow(texture(texture_albedo[i], texCoords).rgb, vec3(2.2));
            float metallic  = texture(texture_metallic[i], texCoords).r;
            float roughness = texture(texture_roughness[i], texCoords).r;
            float ao        = texture(texture_ao[i], texCoords).r;


            // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
            // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
            vec3 F0 = vec3(0.04); 
            F0 = mix(F0, albedo, metallic);

            // reflectance equation
            vec3 Lo = vec3(0.0);

            // calculate per-light radiance
            vec3 L = normalize(lightDirection);
            vec3 H = normalize(V + L);
            vec3 radiance = lightColor;

            // Cook-Torrance BRDF
            float NDF = DistributionGGX(N, H, roughness);   
            float G   = GeometrySmith(N, V, L, roughness);      
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
               
            vec3 nominator    = NDF * G * F; 
            float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
            vec3 specular = nominator / denominator;
            
            // kS is equal to Fresnel
            vec3 kS = F;
            // for energy conservation, the diffuse and specular light can't
            // be above 1.0 (unless the surface emits light); to preserve this
            // relationship the diffuse component (kD) should equal 1.0 - kS.
            vec3 kD = vec3(1.0) - kS;
            // multiply kD by the inverse metalness such that only non-metals 
            // have diffuse lighting, or a linear blend if partly metal (pure metals
            // have no diffuse light).
            kD *= 1.0 - metallic;	  

            // scale light by NdotL
            float NdotL = max(dot(N, L), 0.0);        

            // add to outgoing radiance Lo
            Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again   
            
            // ambient lighting (note that the next IBL tutorial will replace 
            // this ambient lighting with environment lighting).
            vec3 ambient = vec3(1) * albedo * ao;
            
            color[i] = ambient + Lo;

        }
        finalColor = color[4];
    }
    
    // HDR tonemapping
    finalColor = finalColor / (finalColor + vec3(1.0));
    // gamma correct
    finalColor = pow(finalColor, vec3(1.0/2.2));

    FragColor = vec4(finalColor, 1.0);
}