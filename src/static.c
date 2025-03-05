#include "static.h"

void RenderStaticObject(StaticObject* s){
    if(s->type == TYPE_PARTICLE){
        DrawCircleV(s->position, s->Shape.Circle.radius, s->color);
    }
}

void InitStaticObject(StaticObject* s, Vector2 pos, ObjectType type, Color c, Vector2 size, float r){
    s->position = pos;
    s->type = type;
    s->color = c;

    if(type == TYPE_PARTICLE){
        s->Shape.Circle.radius = r;
        s->info = (DebugData){pos, (Vector2){0}, c, r};
    } else if(type == TYPE_AABB){
        s->Shape.Rect.size = size;
    } else {
        perror("ERROR: Not Valid Type");
        return;
    }

    // Methods
    s->Render = RenderStaticObject;
}

bool StaticObjectvsParticle(StaticObject* s, Particle* p){
    if(s->type == TYPE_PARTICLE){
        float r = s->Shape.Circle.radius + p->radius;
        r *= r;

        float distance_squared = powf(p->position.x - s->position.x, 2) + powf(p->position.y - s->position.y, 2);
        return distance_squared < r;
    } else {
        return false;
    }
}