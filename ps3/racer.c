#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define EPSILON 0.0000001
#define SMALL_ADJUST 0.0000001

bool precision_met(double a, double b, double c);
double refine_value(double estimate, int segments, int lengths[segments], int velocities[segments], double duration);
double evaluate_function(double estimate, int segments, int lengths[segments], int velocities[segments]);
double evaluate_derivative(double estimate, int segments, int lengths[segments], int velocities[segments]);

int main()
{
    int segments, duration;
    scanf("%d %d", &segments, &duration);

    int lengths[segments], velocities[segments];
    double guess = 0.0;

    for (int idx = 0; idx < segments; idx++)
    {
        scanf("%d %d", &lengths[idx], &velocities[idx]);
        guess = (-velocities[idx] > guess) ? -velocities[idx] + SMALL_ADJUST : guess + (guess == -velocities[idx] ? SMALL_ADJUST : 0);
    }

    double a, b, c;
    b = refine_value(guess, segments, lengths, velocities, duration);
    a = refine_value(b, segments, lengths, velocities, duration);
    c = refine_value(a, segments, lengths, velocities, duration);

    while (!precision_met(a, c, b))
    {
        b = a;
        a = c;
        c = refine_value(a, segments, lengths, velocities, duration);
    }

    printf("%lf", c);
}

bool precision_met(double a, double b, double c)
{
    return (a - b == 0.0 || a - c == 0.0) ? true : fabs((c - b) / (1.0 - (c - b) / (a - b))) < EPSILON;
}

double refine_value(double estimate, int segments, int lengths[segments], int velocities[segments], double duration)
{
    return estimate - (evaluate_function(estimate, segments, lengths, velocities) - duration) / evaluate_derivative(estimate, segments, lengths, velocities);
}

double evaluate_function(double estimate, int segments, int lengths[segments], int velocities[segments])
{
    double total = 0.0;
    for (int idx = 0; idx < segments; idx++)
    {
        total += lengths[idx] / ((double)velocities[idx] + estimate);
    }
    return total;
}

double evaluate_derivative(double estimate, int segments, int lengths[segments], int velocities[segments])
{
    double total = 0.0;
    for (int idx = 0; idx < segments; idx++)
    {
        total -= lengths[idx] / pow(velocities[idx] + estimate, 2);
    }
    return total;
}
