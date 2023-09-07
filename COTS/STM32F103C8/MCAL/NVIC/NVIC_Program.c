/****************************************************************/
/******* Author    : Mahmoud Abdelraouf Mahmoud *****************/
/******* Date      : 6 Sep 2023                 *****************/
/******* Version   : 0.1                        *****************/
/******* File Name : NVIC_config.h              *****************/
/****************************************************************/

/*****************************< LIB *****************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
/*****************************< MCAL *****************************/
#include "NVIC_interface.h"
#include "NVIC_private.h"
#include "NVIC_config.h"
/*****************************< Function Implementations *****************************/
Std_ReturnType MCAL_NVIC_EnableIRQ(IRQn_Type Copy_IRQn)
{
   Std_ReturnType Local_FunctionStatus = E_NOT_OK;
    
   if(Copy_IRQn < 32)
    {
        NVIC_ISER0 = (1 << Copy_IRQn);
        Local_FunctionStatus = E_OK;
    }
    else if (Copy_IRQn <= 64)
    {
        NVIC_ISER1 = (1 << (Copy_IRQn - 32));
        Local_FunctionStatus = E_OK;
    }
    else if (Copy_IRQn < 96)
    {
        NVIC_ISER2 = (1 << (Copy_IRQn - 64));
        Local_FunctionStatus = E_OK;
    }
    else
    {
        Local_FunctionStatus = E_NOT_OK;
    }
    
    return Local_FunctionStatus;
    
}

Std_ReturnType MCAL_NVIC_DisableIRQ(IRQn_Type Copy_IRQn)
{
    Std_ReturnType Local_FunctionStatus = E_NOT_OK;

   if(Copy_IRQn < 32)
    {
        NVIC_ICER0 = (1 << Copy_IRQn);
        Local_FunctionStatus = E_OK;
    }
    else if (Copy_IRQn <= 64)
    {
        NVIC_ICER1 = (1 << (Copy_IRQn - 32));
        Local_FunctionStatus = E_OK;
    }
    else if (Copy_IRQn < 96)
    {
        NVIC_ICER2 = (1 << (Copy_IRQn - 64));
        Local_FunctionStatus = E_OK;
    }
    else
    {
        Local_FunctionStatus = E_NOT_OK;
    }
    
    return Local_FunctionStatus;
}

Std_ReturnType MCAL_NVIC_SetPendingIRQ(IRQn_Type Copy_IRQn)
{
    Std_ReturnType Local_FunctionStatus = E_NOT_OK;

   if(Copy_IRQn < 32)
    {
        NVIC_ISPR0 = (1 << Copy_IRQn);
        Local_FunctionStatus = E_OK;
    }
    else if (Copy_IRQn <= 64)
    {
        NVIC_ISPR1 = (1 << (Copy_IRQn - 32));
        Local_FunctionStatus = E_OK;
    }
    else if (Copy_IRQn < 96)
    {
        NVIC_ISPR2 = (1 << (Copy_IRQn - 64));
        Local_FunctionStatus = E_OK;
    }
    else
    {
        Local_FunctionStatus = E_NOT_OK;
    }
    
    return Local_FunctionStatus;
}

Std_ReturnType MCAL_NVIC_ClearPendingIRQ(IRQn_Type Copy_IRQn)
{
    Std_ReturnType Local_FunctionStatus = E_NOT_OK;

    if (Copy_IRQn < 32)
    {
        NVIC_ICPR0 = (1U << Copy_IRQn);
        Local_FunctionStatus = E_OK;
    }
    else if (Copy_IRQn < 64)
    {
        NVIC_ICPR1 = (1U << (Copy_IRQn - 32));
        Local_FunctionStatus = E_OK;
    }
    else if (Copy_IRQn < 96)
    {
        NVIC_ICPR2 = (1U << (Copy_IRQn - 64));
        Local_FunctionStatus = E_OK;
    }

    return Local_FunctionStatus;
}

Std_ReturnType MCAL_NVIC_GetPendingIRQ(IRQn_Type Copy_IRQn, u8 *Copy_ReturnPendingFlag)
{
    Std_ReturnType Local_FunctionStatus = E_NOT_OK;

    if (Copy_ReturnPendingFlag != NULL)
    {
        /**< Check the pending flag based on the interrupt number */ 
        if (Copy_IRQn < 32)
        {
            /**< Check the bit corresponding to the interrupt */ 
            if (NVIC_ISPR0 & (1U << Copy_IRQn))
            {
                *Copy_ReturnPendingFlag = 1; /**< Set to 1 if the flag is pending */ 
            }
            else
            {
                *Copy_ReturnPendingFlag = 0; /**< Set to 0 if the flag is not pending */ 
            }
            Local_FunctionStatus = E_OK;
        }
        else if (Copy_IRQn < 64)
        {
            /**< Check the bit corresponding to the interrupt */ 
            if (NVIC_ISPR1 & (1U << (Copy_IRQn - 32)))
            {
                *Copy_ReturnPendingFlag = 1; /**< Set to 1 if the flag is pending */ 
            }
            else
            {
                *Copy_ReturnPendingFlag = 0; /**< Set to 0 if the flag is not pending */ 
            }
            Local_FunctionStatus = E_OK;
        }
        else if(Copy_IRQn < 96)
        {   
            /**< Check the bit corresponding to the interrupt */
            if(NVIC_ISPR2 & (1U << (Copy_IRQn - 64)))
            {
                *Copy_ReturnPendingFlag = 1; /**< Set to 1 if the flag is pending */ 
            }
            else
            {
                *Copy_ReturnPendingFlag = 0; /**< Set to 0 if the flag is not pending */ 
            }
            Local_FunctionStatus = E_OK;
        }
        else
        {
            Local_FunctionStatus = E_NOT_OK;
        }
    }

    return Local_FunctionStatus;
}

Std_ReturnType MCAL_NVIC_xSetPriority(IRQn_Type Copy_IRQn, u32 Copy_Priority)
{
    Std_ReturnType Local_FunctionStatus = E_NOT_OK;

    if (Copy_IRQn < 0 || Copy_IRQn >= NUMBER_OF_INTERRUPTS) /**< Check if IRQn is within valid range */ 
    {
        return Local_FunctionStatus;
    }

    if (Copy_Priority <= NVIC_MAX_PRIORITY) /**< Ensure the priority value is within the valid range (0-255) */ 
    {
        /**< Calculate the register index (IPRx) and bit position within the register */ 
        u8 RegisterIndex = Copy_IRQn / 4;     /**< Divide by 4 to get the register index */  

        /**< Set the priority in the appropriate IPRx register */ 
        NVIC_IPR_BASE_ADDRESS[RegisterIndex] = (Copy_Priority << 4);
        
        /**< Set the group and sub-group priority for interrupt handling in SCB_AIRCR register */
        SCB_AIRCR = _0GROUP_16SUB;

        Local_FunctionStatus = E_OK;
    }

    return Local_FunctionStatus;
}

Std_ReturnType MCAL_NVIC_vSetPriority(IRQn_Type Copy_IRQn, u8 Copy_GroupPriority, u8 Copy_SubPriority)
{
    Std_ReturnType Local_FunctionStatus = E_NOT_OK;
    u8 NVIC_MAX_Group_Priority;
    u8 NVIC_MAX_Sub_Priority;

    #if (PRIORITY_GROUPING == _16GROUP_0SUB)
        NVIC_MAX_Group_Priority = 15;
        NVIC_MAX_Sub_Priority = 0;
    #elif (PRIORITY_GROUPING == _8GROUP_2SUB)
        NVIC_MAX_Group_Priority = 7;
        NVIC_MAX_Sub_Priority = 1;
    #elif (PRIORITY_GROUPING == _4GROUP_4SUB)
        NVIC_MAX_Group_Priority = 3;
        NVIC_MAX_Sub_Priority = 3;
    #elif (PRIORITY_GROUPING == _2GROUP_8SUB)
        NVIC_MAX_Group_Priority = 1;
        NVIC_MAX_Sub_Priority = 7;
    #elif (PRIORITY_GROUPING == _0GROUP_16SUB)
        NVIC_MAX_Group_Priority = 0;
        NVIC_MAX_Sub_Priority = 15;
    #else
        #error "Invalid PRIORITY_GROUPING value. Please choose from _16GROUP_SUB0, _8GROUP_SUB2, _4GROUP_SUB4, _2GROUP_SUB8, or _0GROUP_SUB16."
    #endif

    if (Copy_GroupPriority > NVIC_MAX_Group_Priority || Copy_SubPriority > NVIC_MAX_Sub_Priority)
    {
        /**< Invalid priority value */
        return Local_FunctionStatus;
    }

    u8 Local_Priority = (Copy_SubPriority | (Copy_GroupPriority << (PRIORITY_GROUPING - _16GROUP_0SUB) / 0x100));

    if (Copy_IRQn < 0 || Copy_IRQn >= NUMBER_OF_INTERRUPTS)
    {
        /**< Check if IRQn is within valid range */
        return Local_FunctionStatus;
    }

    /**< Calculate the register index (IPRx) and bit position within the register */
    u8 RegisterIndex = Copy_IRQn / 4; /* Divide by 4 to get the register index */

    /**< Set the priority in the appropriate IPRx register */
    NVIC_IPR_BASE_ADDRESS[RegisterIndex] = (Local_Priority << 4);

    /**< Configure the priority grouping for the Nested Vectored Interrupt Controller (NVIC) */
    SCB_AIRCR = PRIORITY_GROUPING;

    return Local_FunctionStatus;
}

Std_ReturnType MCAL_NVIC_xGetPriority(IRQn_Type IRQn, u8 *Copy_Priority)
{
    Std_ReturnType Local_FunctionStatus = E_NOT_OK;

    if (IRQn < 0 || IRQn > NUMBER_OF_INTERRUPTS)
    {
        /**< Check if IRQn is within valid range */
        return Local_FunctionStatus;
    }

    /**< Calculate the register index (IPRx) and bit position within the register */
    u8 RegisterIndex = IRQn / 4; /* Divide by 4 to get the register index */
    u8 PriorityRegisterValue = NVIC_IPR_BASE_ADDRESS[RegisterIndex];

    /**< Extract the priority from the IPRx register */
    *Copy_Priority = (PriorityRegisterValue >> 4) & 0xFF;

    Local_FunctionStatus = E_OK;

    /**< You may want to return the extracted priority here */
    return Local_FunctionStatus;
}
/*****************************< End of Function Implementations *****************************/
