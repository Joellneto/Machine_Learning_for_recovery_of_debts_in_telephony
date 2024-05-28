#ifndef TerminalApp_h
#define TerminalApp_h

extern KeyPadHelper kp;
extern TelaOLED oled;
extern App app;


enum TerminalAppteps
{
  ExibeMenu,
  AppPedeParametros,
  AppPedeParametro,
  AppExecuta,
  AppExibeResultados
};

class TerminalApp
{
  public:
    TerminalApp()
    {
    }

    ~TerminalApp()
    {
    }

    void Start()
    {
      current_step = ExibeMenu;
      StartStep();
    }

    void StartStep()
    {
      switch (current_step)
      {
        case ExibeMenu:
          char title[255];
          sprintf(title, "Wellcome to %s", app.name);
          oled.drawSimpleText(title, "Press any key");
          break;

        case AppPedeParametros:
            Serial.print("Executando app ");
            par_count = 0;
            if (app.n_inputs > 0)
              current_step = AppPedeParametro;
            else
              current_step = AppExecuta;
           StartStep();
          break;
        case AppPedeParametro:
          char parprompt[255];
          char_count=0;
          sprintf(parprompt,"%d pos - between %.0f and %.0f - # to end",app.inputs[par_count].digits,app.inputs[par_count].min_value,app.inputs[par_count].max_value);
          oled.drawSimpleText(parprompt, app.inputs[par_count].name);
          break;
        case AppExecuta:
          app.Execute();
          current_step = AppExibeResultados;
          StartStep();
          break;
        case AppExibeResultados:
          char result[255];
          if(app.getOutputValue()==1.0)
            strcpy(result,"Yes");
          else
            strcpy(result,"No");
          Serial.print("Result ");
          Serial.print(app.getOutputValue());
          Serial.print(" - valor ");
          Serial.println(result);
          oled.drawSimpleText(app.output.name, result);
          break;
      }
    }

    void Loop()
    {
      int key;
      switch (current_step)
      {
        case ExibeMenu:
          key = kp.Loop();
          if (key > 0)
          {
            Serial.print("Tecla ");
            Serial.println(key);
            delay(500);
            current_step = AppPedeParametros;
            StartStep();
          }
          break;

        case AppPedeParametro:
          key = kp.Loop();
          double val;
          if (key > 0)
          {
            Serial.print("Tecla ");
            Serial.println(key);
            switch (key)
            {
              case '#': // Finalizar
                current_par[char_count++] = '\0';
                val = atof(current_par);
                if(strlen(current_par) != app.inputs[par_count].digits || val < app.inputs[par_count].min_value ||  val > app.inputs[par_count].max_value)
                {
                    // erro de validacao, pede parametro novamente
                    Serial.println("Parametro invalido");
                    current_step = AppPedeParametro;
                }
                else
                {
                  // parametro ok, segue...
                  Serial.print("Parametro ");
                  Serial.print(app.inputs[par_count].name);
                  Serial.print(" - valor ");
                  Serial.println(val);
                  app.setInputValue(app.inputs[par_count].name, val);
                  if (app.n_inputs > par_count + 1)
                    par_count++;
                  else
                    current_step = AppExecuta;
                }
                StartStep();
                break;
             default:
               current_par[char_count++] = key;
               break;
            }
            delay(500);
          }
          break;

        case AppExibeResultados:
          key = kp.Loop();
          if (key > 0)
          {
            Serial.print("Tecla ");
            Serial.println(key);
            delay(500);
            current_step = ExibeMenu;
            StartStep();
          }
          break;
      }
    }

  private:

    TerminalAppteps current_step;
    int par_count;
    char current_par[10];
    int char_count;
};

#endif
