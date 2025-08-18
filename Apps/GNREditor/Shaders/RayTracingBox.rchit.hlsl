// © 2021 NVIDIA Corporation

#include "NRI.hlsl"

NRI_RESOURCE(Buffer<float2>, vertexBuffers[], t, 0, 1);
NRI_RESOURCE(Buffer<uint4>, indexBuffers[], t, 0, 2);

struct Payload
{
    float3 hitValue;
};

struct IntersectionAttributes
{
    float2 barycentrics;
};

[shader( "closesthit" )]
void closest_hit( inout Payload payload : SV_RayPayload, in IntersectionAttributes intersectionAttributes : SV_IntersectionAttributes )
{
    uint instanceID = InstanceID( );
    uint primitiveIndex = PrimitiveIndex( );

    uint3 indices = indexBuffers[instanceID][primitiveIndex].xyz;

    float2 texCoords0 = vertexBuffers[instanceID][indices.x];
    float2 texCoords1 = vertexBuffers[instanceID][indices.y];
    float2 texCoords2 = vertexBuffers[instanceID][indices.z];

    float3 barycentrics;
    barycentrics.yz = intersectionAttributes.barycentrics.xy;
    barycentrics.x = 1.0 - barycentrics.y - barycentrics.z;

    float2 texcoords = barycentrics.x * texCoords0 + barycentrics.y * texCoords1 + barycentrics.z * texCoords2;

    payload.hitValue = float3( texcoords, 0 );
}
