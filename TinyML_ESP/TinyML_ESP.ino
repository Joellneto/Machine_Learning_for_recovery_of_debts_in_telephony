#include "TelaOLED.hpp"
#include "KeyPadHelper.hpp"
#include "App.hpp"
#include "TerminalApp.hpp"
extern "C" void score(double * input, double * output);

KeyPadHelper kp;
TelaOLED oled;
App app;
TerminalApp fluxo;

// Compiling the sketch in ESP32:
// O sketch usa 554737 bytes (42%) de espaço de armazenamento para programas. O máximo são 1310720 bytes.
// Variáveis globais usam 26680 bytes (8%) de memória dinâmica, deixando 301000 bytes para variáveis locais. O máximo são 327680 bytes.


const char *model_definition= "{"
"  \"name\": \"Telephony Debts ML Model\","
"  \"class\": \"Results\","
"  \"fields\": ["
"    {"
"      \"name\": \"TIPO_PES\","
"      \"type\": \"categorical\","
"      \"digits\": 1,"
"      \"min_value\": 0,"
"      \"max_value\": 0"
"    },"
"    {"
"      \"name\": \"SEXO_PES\","
"      \"type\": \"categorical\","
"      \"digits\": 1,"
"      \"min_value\": 0,"
"      \"max_value\": 3"
"    },"
"    {"
"      \"name\": \"ESTADO_CIVIL_PES\","
"      \"type\": \"categorical\","
"      \"digits\": 2,"
"      \"min_value\": 0,"
"      \"max_value\": 11"
"    },"
"    {"
"      \"name\": \"IDADE\","
"      \"type\": \"continuous\","
"      \"digits\": 3,"
"      \"min_value\": 0,"
"      \"max_value\": 111"
"    },"
"    {"
"      \"name\": \"VALOR_TOTAL\","
"      \"type\": \"continuous\","
"      \"digits\": 5,"
"      \"min_value\": 0,"
"      \"max_value\": 30405.89"
"    },"
"    {"
"      \"name\": \"ATRASO\","
"      \"type\": \"continuous\","
"      \"digits\": 3,"
"      \"min_value\": 0,"
"      \"max_value\": 296"
"    },"
"    {"
"      \"name\": \"NOME_TIPE\","
"      \"type\": \"categorical\","
"      \"digits\": 1,"
"      \"min_value\": 0,"
"      \"max_value\": 3"
"    },"
"    {"
"      \"name\": \"TEVE_DEVOL\","
"      \"type\": \"categorical\","
"      \"digits\": 1,"
"      \"min_value\": 0,"
"      \"max_value\": 1"
"    },"
"    {"
"      \"name\": \"CIDADES\","
"      \"type\": \"categorical\","
"      \"digits\": 4,"
"      \"min_value\": 0,"
"      \"max_value\": 2598"
"    },"
"    {"
"      \"name\": \"UF\","
"      \"type\": \"categorical\","
"      \"digits\": 2,"
"      \"min_value\": 0,"
"      \"max_value\": 26"
"    }"
"  ]"
"}";

void setup()
{
  Serial.begin(115200);

  kp.Start();
  oled.Start();
  app.Deserialize(model_definition);
  fluxo.Start();
  
  Serial.println("Tiny ML Project");
}

void testModel()
{
  Serial.println("Testing model...");
  double input[10] { 0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00 };
  double output[2];
  score(input,output);
  Serial.print("Result: ");
  Serial.print(output[0]);
  Serial.print(",");
  Serial.println(output[1]);
}

void loop()
{
  fluxo.Loop();
  delay(10);
}
