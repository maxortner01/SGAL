#include <SGAL/SGAL.h>

#include <iostream>

namespace sgal
{

    SingleModel::SingleModel()  :
		model(&rawModel)
	{   
        
    }

    void SingleModel::draw(const Surface* surface, const RenderContext* rc) const
    {
        surface->draw(model, rc);
    }

}