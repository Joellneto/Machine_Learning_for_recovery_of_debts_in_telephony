import azure.functions as func
import logging
import pandas as pd
import joblib
import json
import numpy as np

from inference_schema.schema_decorators import input_schema, output_schema
from inference_schema.parameter_types.numpy_parameter_type import NumpyParameterType
from inference_schema.parameter_types.pandas_parameter_type import PandasParameterType
from inference_schema.parameter_types.standard_py_parameter_type import StandardPythonParameterType

app = func.FunctionApp(http_auth_level=func.AuthLevel.ANONYMOUS)

model = joblib.load('Debts-AutoML-bestmodel.pkl')
data_sample = PandasParameterType(pd.DataFrame({"TIPO_PES": pd.Series(["example_value"], dtype="object"), "SEXO_PES": pd.Series(["example_value"], dtype="object"), "ESTADO_CIVIL_PES": pd.Series(["example_value"], dtype="object"), "IDADE": pd.Series([0.0], dtype="float32"), "VALOR_TOTAL": pd.Series([0.0], dtype="float32"), "ATRASO": pd.Series([0], dtype="int16"), "NOME_TIPE": pd.Series(["example_value"], dtype="object"), "TEVE_DEVOL": pd.Series([0], dtype="int8"), "CIDADES": pd.Series(["example_value"], dtype="object"), "UF": pd.Series(["example_value"], dtype="object")}))
input_sample = StandardPythonParameterType({'data': data_sample})
method_sample = StandardPythonParameterType("predict")
sample_global_params = StandardPythonParameterType({"method": method_sample})

result_sample = NumpyParameterType(np.array([0]))
output_sample = StandardPythonParameterType({'Results':result_sample})

@app.route(route="http_trigger")
def http_trigger(req: func.HttpRequest) -> func.HttpResponse:
    logging.info('Python HTTP trigger function processed a request.')

    data = req.get_json()
    logging.info('Request: ' + str(data))

    # If the data is not empty, convert to a pandas DataFrame 
    if data is not None:
        return json.dumps(run(data["Inputs"]))
    else:
        # If no JSON data is recieved, print error response
        return func.HttpResponse(
             "Please pass a properly formatted JSON object to the API",
             status_code=400
        )


@input_schema('GlobalParameters', sample_global_params, convert_to_provided_type=False)
@input_schema('Inputs', input_sample)
@output_schema(output_sample)
def run(Inputs, GlobalParameters={"method": "predict"}):
    data = Inputs['data']
    if GlobalParameters.get("method", None) == "predict_proba":
        result = model.predict_proba(data)
    elif GlobalParameters.get("method", None) == "predict":
        result = model.predict(data)
    else:
        raise Exception(f"Invalid predict method argument received. GlobalParameters: {GlobalParameters}")
    if isinstance(result, pd.DataFrame):
        result = result.values
    return {'Results':result.tolist()}

