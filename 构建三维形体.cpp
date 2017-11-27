#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkTextMapper.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkArrowSource.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkDiskSource.h>
#include <vtkLineSource.h>
#include <vtkRegularPolygonSource.h>
#include <vtkSphereSource.h>
#include <vector>

int main()
{
    std::vector<vtkSmartPointer<vtkPolyDataAlgorithm> > geometricObjectSources;

    geometricObjectSources.push_back(vtkSmartPointer<vtkArrowSource>::New());//箭头
    geometricObjectSources.push_back(vtkSmartPointer<vtkConeSource>::New());//锥体
    geometricObjectSources.push_back(vtkSmartPointer<vtkCubeSource>::New());//立方体
    geometricObjectSources.push_back(vtkSmartPointer<vtkCylinderSource>::New());//缸体
    geometricObjectSources.push_back(vtkSmartPointer<vtkDiskSource>::New());//转盘
    geometricObjectSources.push_back(vtkSmartPointer<vtkLineSource>::New());//直线
    geometricObjectSources.push_back(vtkSmartPointer<vtkRegularPolygonSource>::New());//正多面体
    geometricObjectSources.push_back(vtkSmartPointer<vtkSphereSource>::New());//球体

    std::vector<vtkSmartPointer<vtkRenderer> > renderers;
    std::vector<vtkSmartPointer<vtkPolyDataMapper> > mappers;
    std::vector<vtkSmartPointer<vtkActor> > actors;
    std::vector<vtkSmartPointer<vtkTextMapper> > textmappers;
    std::vector<vtkSmartPointer<vtkActor2D> > textactors;

    for (unsigned int i = 0; i < geometricObjectSources.size(); i++)
    {
        geometricObjectSources[i]->Update();

        mappers.push_back(vtkSmartPointer<vtkPolyDataMapper>::New());
mappers[i]->SetInputConnection(geometricObjectSources[i]->GetOutputPort());

        actors.push_back(vtkSmartPointer<vtkActor>::New());
        actors[i]->SetMapper(mappers[i]);

        textmappers.push_back(vtkSmartPointer<vtkTextMapper>::New());
        textmappers[i]->SetInput(geometricObjectSources[i]->GetClassName()); 

        textactors.push_back(vtkSmartPointer<vtkActor2D>::New());
        textactors[i]->SetMapper(textmappers[i]);
        textactors[i]->SetPosition(10, 10);
    }

    // 设置格网承载形体
    int gridCols = 3;
    int gridRows = 3;
    // 定义渲染范围
    int rendererSize = 200;

    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(rendererSize*gridCols, rendererSize*gridRows);

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();

    for (double row = 0; row < gridRows; row++)
    {
        for (double col = 0; col < gridCols; col++)
        {
            double index = row*gridCols + col;

            //为每个cell定义渲染器
            vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
            renderer->SetBackground(.1, .2, .3);
            double viewport[4] = {
                ((col)* rendererSize) / (gridCols * rendererSize),
                ((gridRows - (row + 1)) * rendererSize) / (gridRows * rendererSize),
                ((col + 1)              * rendererSize) / (gridCols * rendererSize),
                ((gridRows - (row)) * rendererSize) / (gridRows * rendererSize) };
            renderer->SetViewport(viewport);
            if (index < geometricObjectSources.size())
            {
                renderer->AddActor(actors[index]);
                renderer->AddActor(textactors[index]);
            }
            renderWindow->AddRenderer(renderer);
        }
    }

    vtkSmartPointer<vtkRenderWindowInteractor> interactor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);
    renderWindow->Render();
    interactor->Start();

    return 0;
}