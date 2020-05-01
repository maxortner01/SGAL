#include <SGAL/SGAL.h>

namespace sgal
{

    SingleModel::SingleModel()  :
		model(&rawModel)
	{   }

    void SingleModel::draw(const Surface* surface, const RenderContext* rc) const
    {
        surface->draw(model, rc);
    }

}