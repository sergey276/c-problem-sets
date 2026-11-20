#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define SCALE_FACTOR 1000.0
#define BOX_SIZE 100.0
#define BOX_VOLUME (BOX_SIZE * BOX_SIZE * BOX_SIZE)
#define TOLERANCE 0.0000001
#define PI 3.14159265358979323846

struct Ball {
    double coord_x, coord_y, coord_z;
    double rad;
};

double computeSegmentVolume(const struct Ball *ball, double depth, bool derivative);
double computeLayerVolume(double base, double height, int total, struct Ball balls[total], bool derivative);
bool checkConvergence(double previous, double current, double next);
double adjustLayerHeight(double base, double height, int total, struct Ball balls[total], double expectedVolume);
double calculateIntersection(double base, double height, const struct Ball *ball, bool derivative);
double findBallVolume(const struct Ball *ball);

int main()
{
    int totalBalls, totalLayers;
    scanf("%d %d", &totalBalls, &totalLayers);

    struct Ball balls[totalBalls];
    double freeVolume = BOX_VOLUME;

    int index = 0;
    while (index < totalBalls)
    {
        scanf("%lf %lf %lf %lf", &balls[index].rad, &balls[index].coord_x, &balls[index].coord_y, &balls[index].coord_z);
        balls[index].rad = balls[index].rad / SCALE_FACTOR;
        balls[index].coord_x = balls[index].coord_x / SCALE_FACTOR;
        balls[index].coord_y = balls[index].coord_y / SCALE_FACTOR;
        balls[index].coord_z = balls[index].coord_z / SCALE_FACTOR;
        freeVolume = freeVolume - findBallVolume(&balls[index]);
        index++;
    }

    double layerVolume = freeVolume / (double)totalLayers;
    double layerThickness = BOX_SIZE / (double)totalLayers;

    double prevValue, currentValue, nextValue;
    double currentLayer = 0.0;

    int layer = 0;
    while (layer < totalLayers)
    {
        prevValue = adjustLayerHeight(currentLayer, layerThickness, totalBalls, balls, layerVolume);
        currentValue = adjustLayerHeight(currentLayer, prevValue, totalBalls, balls, layerVolume);
        nextValue = adjustLayerHeight(currentLayer, currentValue, totalBalls, balls, layerVolume);

        for (; !checkConvergence(prevValue, currentValue, nextValue);)
        {
            prevValue = currentValue;
            currentValue = nextValue;
            nextValue = adjustLayerHeight(currentLayer, currentValue, totalBalls, balls, layerVolume);
        }

        printf("%lf\n", nextValue);
        currentLayer = currentLayer + nextValue;
        layer++;
    }
}

double findBallVolume(const struct Ball *ball)
{
    double radiusCubed = pow(ball->rad, 3.0);
    double volume = (4.0 / 3.0) * PI * radiusCubed;
    return volume;
}

double computeSegmentVolume(const struct Ball *ball, double depth, bool derivative)
{
    if (derivative == true)
    {
        double term1 = 2.0 * ball->rad;
        double term2 = term1 - depth;
        double result = PI * depth * term2;
        return result;
    }
    else
    {
        double term1 = depth * depth;
        double term2 = 3.0 * ball->rad - depth;
        double result = (PI * term1 * term2) / 3.0;
        return result;
    }
}

double calculateIntersection(double base, double height, const struct Ball *ball, bool derivative)
{
    double topLimit = base + height;
    double ballTop = ball->coord_z + ball->rad;
    double ballBottom = ball->coord_z - ball->rad;
    
    if (ballTop <= base)
    {
        return 0.0;
    }
    if (ballBottom >= topLimit)
    {
        return 0.0;
    }
    
    double overlap = 0.0;
    if (derivative == false)
    {
        overlap = findBallVolume(ball);
    }
    
    if (ballTop > topLimit)
    {
        double excess = ballTop - topLimit;
        overlap = overlap - computeSegmentVolume(ball, excess, derivative);
    }
    
    if (ballBottom < base)
    {
        double excess = base - ballBottom;
        overlap = overlap - computeSegmentVolume(ball, excess, derivative);
    }
    
    return overlap;
}

double computeLayerVolume(double base, double height, int total, struct Ball balls[total], bool derivative)
{
    double boxBaseVolume = BOX_SIZE * BOX_SIZE;
    double volume = boxBaseVolume * (derivative ? 1.0 : height);
    
    int index = 0;
    while (index < total)
    {
        volume = volume - calculateIntersection(base, height, &balls[index], derivative);
        index++;
    }
    
    return volume;
}

double adjustLayerHeight(double base, double height, int total, struct Ball balls[total], double expectedVolume)
{
    double layerVolume = computeLayerVolume(base, height, total, balls, false);
    double derivativeVolume = computeLayerVolume(base, height, total, balls, true);
    double correction = (layerVolume - expectedVolume) / derivativeVolume;
    double newHeight = height - correction;
    return newHeight;
}

bool checkConvergence(double previous, double current, double next)
{
    if ((current - previous) == 0.0)
    {
        return true;
    }
    
    if ((current - next) == 0.0)
    {
        return true;
    }
    
    double numerator = next - current;
    double denominator = current - previous;
    double fraction = numerator / denominator;
    double adjustment = 1.0 - fraction;
    double diffRatio = fabs(numerator / adjustment);
    
    if (diffRatio < TOLERANCE)
    {
        return true;
    }
    else
    {
        return false;
    }
}
