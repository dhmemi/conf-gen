import * as React from 'react';
import ReactDOM from 'react-dom';
import { StyledEngineProvider } from '@mui/material/styles';
import ParamTree from './items/ItemGroup';
import param_data from "./conf-1.json";


ReactDOM.render(
  <StyledEngineProvider injectFirst>
    <ParamTree data={param_data} />
  </StyledEngineProvider>,
  document.querySelector("#root")
);