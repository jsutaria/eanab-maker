
Here is a suggested outline for your proposal. You don't need to follow this
outline, but it may help you focus your project.

* Project title
* Team members
* Description (1 pp describing the basic idea)
* Hardware required (itemize what hardware, if any, you need)
* Funds requested (the amount you will want to be reimbursed, remember you have
  a $20 budget per person)
* Major task breakdown and who is responsible for each task
* Goals for next week
* References and resources used

Keep the proposal short and to the point.

We suggest you try to get all the major hardware components working by the
first week. That will leave the following week for putting the components
together into a working system.


# EANAB Maker

## Description:

We’re creating a system to automatically pour mixed drinks based on user preferences.

Users can enter their preferences into a portal authenticated with their Stanford ID card, which will be then sent to a Raspberry Pi that operates the actual drink maker. When the user swipes their Stanford ID on the drink maker Raspberry Pi, the drink maker retrieves the user’s preferences to create their designated mixed drink. Before making the drink, the drink maker breathalyzes the user to ensure the user is legally sober before consuming any of the beverage. The drink maker can control the amount of liquid poured from a maximum of four different components into a cup. The drink maker will also be able to deliver cups from a dispenser and move a filled cup out of the filling position using a conveyor belt.

## Responsibilities:

Ankush: Develop software to control breathalyzer and valves and make tubing system to channel liquids from bottles to cup filling station with controlled liquid flow

Jainil: Develop software to control conveyor belt and cup dispenser and make cup dispensing system and conveyor belt

Theo: Develop software to record user preferences, send user preferences to Raspberry Pi, develop protocol to send user preferences pi->pi wirelessly, build out Stanford ID authentication system using magstripe readers over PS/2

## Midpoint Milestones:

1. Build out communication protocol to send user preferences from pi->pi and build out Stanford ID authentication system.
2. Create hardware to control the amount of each component poured into the drink and cup dispensing mechanism.
3. Build out software to control valves for the component dispenser.

## Resources/materials:
* Breathalyzer
* 2x Magstripe readers (PS/2)
* 4x Solenoid valves
* Clear tubing
* 4x Bottle spouts
* Red solo cups
* Beverage components
* 3x Servo Motors
* 3x Steper Motors
* 3D-printed drink dispenser and conveyor belt
