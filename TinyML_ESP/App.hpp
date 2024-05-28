#ifndef App_h
#define App_h

#include <ArduinoJson.h>
extern "C" void score(double * input, double * output);

typedef struct AppInput
{
  char name[64];
  char type[15];
  int digits;
  double min_value;
  double max_value;
  double value;
};

typedef struct AppOutput
{
  char name[64];
  double value;
};

#define MAXINPUTS 20

class App
{

public:

	void create(const char* name)
	{
		strcpy(this->name,name);
		n_inputs=0;
	}

	bool addInput(const char* name,const char* type,int digits, double min_value,double max_value)
	{
    if(n_inputs<MAXINPUTS)
    {
  		strcpy(inputs[n_inputs].name,name);
  		strcpy(inputs[n_inputs].type,type);
      inputs[n_inputs].digits = digits;
      inputs[n_inputs].min_value = min_value;
      inputs[n_inputs].max_value = max_value;
      inputs[n_inputs].value=0;
  		n_inputs++;
      return true;
    }
    else
      return false;
	}

	void addOutput(const char* name)
	{
		strcpy(output.name,name);
    output.value=0.0;
	}

	void Deserialize(const char* definition)
	{
    StaticJsonDocument<2500> doc;
    deserializeJson(doc, definition);

    strcpy(name,(const char*)doc["name"]);
  
		for(int i=0;i<doc["fields"].size();i++)
			addInput((const char*)doc["fields"][i]["name"], (const char*)doc["fields"][i]["type"], (int)doc["fields"][i]["digits"], (double)doc["fields"][i]["min_value"], (double)doc["fields"][i]["max_value"]);

		addOutput((const char*)doc["class"]);
	}

  void setInputValue(char* name,double val)
  {
    for(int i=0;i<n_inputs;i++)
    {
      if(strcmp(inputs[i].name,name)==0)
      {
          inputs[i].value = val;
          return;
      }
    }
  }

  void setOutputValue(double val)
  {
    output.value = val;
  }

  double getOutputValue()
  {
     return output.value;
  }

  void Execute()
  {
    double model_inputs[MAXINPUTS];
    for(int i=0;i<n_inputs;i++)
    {
        model_inputs[i] = inputs[i].value;
    }
    double model_output[2];
    score(model_inputs,model_output);
    if(model_output[0]>model_output[1])
      setOutputValue(0.0);
    else
      setOutputValue(1.0);
  }

public:
	char name[64];
	AppInput inputs[MAXINPUTS];
	AppOutput output;
	int n_inputs;
};

#endif
