#include "baro_fusion.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdlib.h>


//Faz a mediana

static int32_t median3(
    int32_t a,
    int32_t b,
    int32_t c
)
{
    int32_t t;

    if(a>b)
    {
        t=a;
        a=b;
        b=t;
    }

    if(b>c)
    {
        t=b;
        b=c;
        c=t;
    }

    if(a>b)
    {
        t=a;
        a=b;
        b=t;
    }

    return b;
}


// Calculo do peso e atribuicao

static int32_t calc_weight(
    int32_t d
)
{
    if(d<20)
        return 100;

    if(d<50)
        return 70;

    if(d<100)
        return 30;

    return 0;
}


//Iniciar

void baro_fusion_init(
    baro_fusion_t *f
)
{
    f->b1.enabled=false;
    f->b1.pressure=0;
    f->b1.ema=0;
    f->b1.diff=0;
    f->b1.weight=0;

    f->b2.enabled=false;
    f->b2.pressure=0;
    f->b2.ema=0;
    f->b2.diff=0;
    f->b2.weight=0;

    f->b3.enabled=false;
    f->b3.pressure=0;
    f->b3.ema=0;
    f->b3.diff=0;
    f->b3.weight=0;

    f->median=0;

    f->fused_pressure=0;

    f->altitude=0;

    f->altitude_offset=0;

    f->altitude_relative=0;
}


//Atualizar todos os valores 

void baro_fusion_update(
    baro_fusion_t *f
)
{
    /*
    Ler Pressão
    */

    if(f->b1.enabled)
    {
        f->b1.pressure =
            (int32_t)
            bmp280_get_pressure(
                &f->b1.bmp
            );
    }

    if(f->b2.enabled)
    {
        f->b2.pressure =
            (int32_t)
            bmp280_get_pressure(
                &f->b2.bmp
            );
    }

    if(f->b3.enabled)
    {
        f->b3.pressure =
            (int32_t)
            bmp280_get_pressure(
                &f->b3.bmp
            );
    }


    /*
    Cálculo da média móvel exponencial
    EMA ou MME= Peso Novo * Leitura nova + Peso antigo * leitura antiga
    
    */

    if(f->b1.enabled)
    {
        if(f->b1.ema==0)
            f->b1.ema=f->b1.pressure;
        else
            f->b1.ema=
                (9*f->b1.ema+f->b1.pressure)/10;
    }

    if(f->b2.enabled)
    {
        if(f->b2.ema==0)
            f->b2.ema=f->b2.pressure;
        else
            f->b2.ema=
                (9*f->b2.ema + f->b2.pressure)/10;
    }

    if(f->b3.enabled)
    {
        if(f->b3.ema==0)
            f->b3.ema=f->b3.pressure;
        else
            f->b3.ema= (9*f->b3.ema+f->b3.pressure)/10;
    }


    //Encontra a mediana

    f->median=
        median3(
            f->b1.ema,
            f->b2.ema,
            f->b3.ema
        );


    // Diferenca da mediana

    f->b1.diff=
        abs(
            f->b1.ema-
            f->median
        );

    f->b2.diff=
        abs(
            f->b2.ema-
            f->median
        );

    f->b3.diff=
        abs(
            f->b3.ema-
            f->median
        );


   // Cálculo dos pesos

    if(f->b1.enabled)
    {
        f->b1.weight=
            calc_weight(
                f->b1.diff
            );
    }
    else
    {
        f->b1.weight=0;
    }

    if(f->b2.enabled)
    {
        f->b2.weight=
            calc_weight(
                f->b2.diff
            );
    }
    else
    {
        f->b2.weight=0;
    }

    if(f->b3.enabled)
    {
        f->b3.weight=
            calc_weight(
                f->b3.diff
            );
    }
    else
    {
        f->b3.weight=0;
    }


    /*Cálculo da fusão dos sensores: média móvel exponecial --> calcula mediana 
    --> desvio da mediana e atribuição de peso --> fusão ponderada
    */

    int32_t sumWeight=

        f->b1.weight+

        f->b2.weight+

        f->b3.weight;

    if(sumWeight>0)
    {
        f->fused_pressure=

        (
            f->b1.ema * f->b1.weight +
            f->b2.ema * f->b2.weight +
            f->b3.ema * f->b3.weight
        )

        /

        sumWeight;
    }


    //Altitude

    f->altitude=
        bmp280_pressure_to_altitude(
            (float)
            f->fused_pressure
        );

    f->altitude_relative=
        f->altitude-
        f->altitude_offset;
}


//Calibrar altitude zero

void baro_fusion_calibrate_zero(
    baro_fusion_t *f,
    int samples,
    int delay_ms
)
{
    float sum=0;

    for(int i=0;i<samples;i++)
    {
        baro_fusion_update(
            f
        );

        sum +=
            f->altitude;

        vTaskDelay(
            pdMS_TO_TICKS(
                delay_ms
            )
        );
    }

    f->altitude_offset=
        sum/
        samples;

    f->altitude_relative=0;
}