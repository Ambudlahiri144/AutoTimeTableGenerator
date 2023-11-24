# AutoTimeTableGenerator

This project executes for generating schedule grid by using Genetic Algorithm.
There is a description below.
1.	Main Setup
    
It is possible to import previously made configuration by pushing special button “Import”. 
Also user can start to create the new configuration with settings by choosing “Time period” (from 2 days and up to 12 days) and “The number of pairs per day” (from 2 pairs and up to 8 pairs). “Next Page” will move to the next settings.

 ![изображение](https://github.com/Firyst/GeneticTimetables/assets/113290258/4dc14ceb-cf55-43d1-b338-fe1b8a6c48df)

2.	Input Data
   
Firstly there is just empty window. 

![изображение](https://github.com/Firyst/GeneticTimetables/assets/113290258/2763a7e4-2f9e-436a-95b4-05e3d85e0d4a)

After pushing the button “Add” one more line will appear. There are two strings: first one is for name of the subject, the second one is for teacher’s name. Spin box responsible for the number of pairs for the subject.

 ![изображение](https://github.com/Firyst/GeneticTimetables/assets/113290258/2ca12882-41c3-44ce-93c8-007d5a1b3af5)

Button “Edit” open new dialog window, where is a table to select preferable days and pairs by putting a tick. The extreme boxes mark for the entire line or column. Also all boxes can be chosen with upper left box. Button “Delete” destroys the record.

![изображение](https://github.com/Firyst/GeneticTimetables/assets/113290258/f7509742-70b9-4488-83e6-88555e29b936)


 
3.	Criteria
   
These parameters define importance of different rules for generating schedule grid. There are default values, but user can adjust to himself.

Conflicts are recommended not be changed to avoid problems with overlays or non-compliance with the norms for pairs. Actually this slider has interval from 10x and up to 100x value.

Time bounds is responsible for importance of accordance to the selected preferable days and pairs. Values are between 0x and 10x.

Repeats can accept both positive and negative values (from -5x and up to 5x). With positive values the program will try to avoid repeating pairs in a row. If the parameter is negative, the program will try to group duplicate pairs.

Gaps parameter is responsible for breaks between pairs. Value 0x means, that it does not matter, 10x – program will avoid breaks as much as possible.
Week balance distributes the number of pairs by week (mean square deviation) from 0x and up to 10x.

Diversity means variety of subjects per day. Positive value of the parameter allows diversifying the day with different disciplines. Negative value will generate not multifarious days.

![изображение](https://github.com/Firyst/GeneticTimetables/assets/113290258/60047f9e-9b7d-4a81-9401-31e9aabd3574)

 
4.	Parameters
   
Warning! Misunderstanding the meaning of these parameters will influence on the result not in a good way. Default values are good enough to receive timetable.

Generations defines the number of generated generations (stars from 500 and ends in 100000)

Population size  actually is the size of this generation, the interval is from 50 and up to 2500.

Crossover chance is responsible for crossings and has percentage calculation.

Mutation chance is responsible for chance of mutation and has percentage calculation.

Mutation threshold defines how much the growth of generations must be slow down in order for mutations to begin to be applied (between 0x and 1x). 

![изображение](https://github.com/Firyst/GeneticTimetables/assets/113290258/c15dfffa-22db-459e-a593-573c4da48ab9)

5.	Generate
   
Firstly there is just empty window.

 ![изображение](https://github.com/Firyst/GeneticTimetables/assets/113290258/5307e183-e759-4c6b-bbae-c418c63e947a)

After pushing the button “Run” the process will start. Working time is counted, also it is shown time left and status bar. 
Shown timetable is the most successful. In addition, other successful results can be shown by increasing the spin box of number result. 
Timetable will be saved with pushing button “Export”. If user wants to take configuration, it also can be made by button “Save configuration”.
 
![изображение](https://github.com/Firyst/GeneticTimetables/assets/113290258/28de482f-c960-463f-bca6-0f874bfff1ff)


 

