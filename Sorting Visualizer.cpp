#include<SDL.h>
#include<iostream>
#include<limits>
#include<time.h>
#include<string>
using namespace std;

const int SCREEN_WIDTH=910;
const int SCREEN_HEIGHT=750;

const int arrSize=130;
const int rectSize=7;

int arr[arrSize];
int Barr[arrSize];

SDL_Window* window=NULL;
SDL_Renderer* renderer=NULL;

bool complete=false;

bool init()
{
    bool success=true;
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        cout<<"Couldn't initialize SDL. SDL_Error: "<<SDL_GetError();
        success=false;
    }
    else
    {
        if(!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
        {
            cout<<"Warning: Linear Texture Filtering not enabled.\n";
        }

        window=SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window==NULL)
        {
            cout<<"Couldn't create window. SDL_Error: "<<SDL_GetError();
            success=false;
        }
        else
        {
            renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer==NULL)
            {
                cout<<"Couldn't create renderer. SDL_Error: "<<SDL_GetError();
                success=false;
            }
        }
    }

    return success;
}

void close()
{
    SDL_DestroyRenderer(renderer);
    renderer=NULL;

    SDL_DestroyWindow(window);
    window=NULL;

    SDL_Quit();
}

void visualize(int x=-1, int y=-1, int z=-1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int j=0;
    for(int i=0; i<=SCREEN_WIDTH-rectSize; i+=rectSize)
    {
        SDL_PumpEvents();

        SDL_Rect rect={i, 0, rectSize, arr[j]};
        if(complete)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if(j==x || j==z)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if(j==y)
        {
            SDL_SetRenderDrawColor(renderer, 165, 105, 189, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

int partition_array(int a[], int si, int ei)
{
    int count_small=0;

    for(int i=(si+1);i<=ei;i++)
    {
        if(a[i]<=a[si])
        {
            count_small++;
        }
    }
    int c=si+count_small;
    int temp=a[c];
    a[c]=a[si];
    a[si]=temp;
    visualize(c, si);

    int i=si, j=ei;

    while(i<c && j>c)
    {
        if(a[i]<= a[c])
        {
            i++;
        }
        else if(a[j]>a[c])
        {
            j--;
        }
        else
        {
            int temp_1=a[j];
            a[j]=a[i];
            a[i]=temp_1;

            visualize(i, j);
            SDL_Delay(70);

            i++;
            j--;
        }
    }
    return c;
}

void quickSort(int a[], int si, int ei)
{
    if(si>=ei)
    {
        return;
    }

    int c=partition_array(a, si, ei);
    quickSort(a, si, c-1);
    quickSort(a, c+1, ei);

}

void merge2SortedArrays(int a[], int si, int ei)
{
    int size_output=(ei-si)+1;
    int* output=new int[size_output];

    int mid=(si+ei)/2;
    int i=si, j=mid+1, k=0;
    while(i<=mid && j<=ei)
    {
        if(a[i]<=a[j])
        {
            output[k]=a[i];
            visualize(i, j);
            i++;
            k++;
        }
        else
        {
            output[k]=a[j];
            visualize(i, j);
            j++;
            k++;
        }

    }
    while(i<=mid)
    {
        output[k]=a[i];
        visualize(-1, i);
        i++;
        k++;
    }
    while(j<=ei)
    {
        output[k]=a[j];
        visualize(-1, j);
        j++;
        k++;
    }
    int x=0;
    for(int l=si; l<=ei; l++)
    {
        a[l]=output[x];
        visualize(l);
        SDL_Delay(15);
        x++;
    }
    delete []output;
}

void mergeSort(int a[], int si, int ei)
{
    if(si>=ei)
    {
        return;
    }
    int mid=(si+ei)/2;

    mergeSort(a, si, mid);
    mergeSort(a, mid+1, ei);

    merge2SortedArrays(a, si, ei);
}

void bubbleSort()
{
    for(int i=0; i<arrSize-1; i++)
    {
        for(int j=0; j<arrSize-1-i; j++)
        {
            if(arr[j+1]<arr[j])
            {
                int temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;

                visualize(j+1, j, arrSize-i);
            }
            SDL_Delay(1);
        }
    }
}

void insertionSort()
{
    for(int i=1; i<arrSize; i++)
    {
        int j=i-1;
        int temp=arr[i];
        while(j>=0 && arr[j]>temp)
        {
            arr[j+1]=arr[j];
            j--;

            visualize(i, j+1);
            SDL_Delay(5);
        }
        arr[j+1]=temp;
    }
}

void loadArr()
{
    memcpy(arr, Barr, sizeof(int)*arrSize);
}

void randomizeAndSaveArray()
{
    unsigned int seed=(unsigned)time(NULL);
    srand(seed);
    for(int i=0; i<arrSize; i++)
    {
        int random=rand()%(SCREEN_HEIGHT);
        Barr[i]=random;
    }
}

void execute()
{
    if(!init())
    {
        cout<<"SDL Initialization Failed.\n";
    }
    else
    {
        randomizeAndSaveArray();
        loadArr();

        SDL_Event e;
        bool quit=false;
        while(!quit)
        {
            while(SDL_PollEvent(&e)!=0)
            {
                if(e.type==SDL_QUIT)
                {
                    quit=true;
                    complete=false;
                }
                else if(e.type==SDL_KEYDOWN)
                {
                    switch(e.key.keysym.sym)
                    {
                        case(SDLK_q):
                            quit=true;
                            complete=false;
                            cout<<"\nEXITING SORTING VISUALIZER.\n";
                            break;
                        case(SDLK_0):
                            randomizeAndSaveArray();
                            complete=false;
                            loadArr();
                            cout<<"\nNEW RANDOM LIST GENERATED.\n";
                            break;
                        case(SDLK_1):
                            loadArr();
                            cout<<"\nINSERTION SORT STARTED.\n";
                            complete=false;
                            insertionSort();
                            complete=true;
                            cout<<"\nINSERTION SORT COMPLETE.\n";
                            break;
                        case(SDLK_2):
                            loadArr();
                            cout<<"\nBUBBLE SORT STARTED.\n";
                            complete=false;
                            bubbleSort();
                            complete=true;
                            cout<<"\nBUBBLE SORT COMPLETE.\n";
                            break;
                        case(SDLK_3):
                            loadArr();
                            cout<<"\nMERGE SORT STARTED.\n";
                            complete=false;
                            mergeSort(arr, 0, arrSize-1);
                            complete=true;
                            cout<<"\nMERGE SORT COMPLETE.\n";
                            break;
                        case(SDLK_4):
                            loadArr();
                            cout<<"\nQUICK SORT STARTED.\n";
                            complete=false;
                            quickSort(arr, 0, arrSize-1);
                            complete=true;
                            cout<<"\nQUICK SORT COMPLETE.\n";
                            break;
                    }
                }
            }
            visualize();
        }
        close();
    }
}

bool controls()
{
    cout <<"Giving repetitive commands may cause delay in visualizer and it may behave unexpectedly.\n\n"
         <<"Available Controls inside Sorting Visualizer:-\n"
         <<"    Use 0 to Generate a different randomized list.\n"
         <<"    Use 1 to start Insertion Sort Algorithm.\n"
         <<"    Use 2 to start Bubble Sort Algorithm.\n"
         <<"    Use 3 to start Merge Sort Algorithm.\n"
         <<"    Use 4 to start Quick Sort Algorithm.\n"
         <<"    Use q to exit out of Sorting Visualizer\n\n"
         <<"PRESS ENTER TO START SORTING VISUALIZER...\n\n"
         <<"Or type -1 and press ENTER to quit the program.";

    string s;
    getline(cin, s);
    if(s=="-1")
    {
        return false;
    }
    //else if(s=="\n")
    //{
    //    return true;
    //}
    return true;
}

void intro()
{
    string s;
    getline(cin, s);
    if(s=="\n")
    {
        return;
    }
}

int main(int argc, char* args[])
{
    intro();

    while(1)
    {
        cout<<'\n';
        if(controls())
            execute();
        else
        {
            cout<<"\nEXITING PROGRAM.\n";
            break;
        }
    }

    return 0;
}
