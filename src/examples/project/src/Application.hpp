#include <ttb/project/Application.hpp>


class Application : public ttb::Application
{
public:
    static Application& instance();

private:
    virtual ttb::WindowRequest windowRequest() const override;
    virtual void init() override;
    virtual void destroy() override;
    virtual void update( float frameTime ) override;
    virtual void draw() const override;
};
