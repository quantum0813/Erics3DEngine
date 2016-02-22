#ifndef SCENE_HPP
#define SCENE_HPP

class Scene {
    public:
        Scene() {};
        ~Scene() {};
        virtual void initScene() = 0;
        virtual void render() = 0;
        virtual void cleanup() = 0;
};

#endif //SCENE_HPP