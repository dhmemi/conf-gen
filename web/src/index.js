import * as React from 'react';
import ReactDOM from 'react-dom';
import Button from '@mui/material/Button';
import Switch from '@mui/material/Switch';
import FormGroup from '@mui/material/FormGroup';
import FormControlLabel from '@mui/material/FormControlLabel';
import FormControl from '@mui/material/FormControl';
import FormLabel from '@mui/material/FormLabel';

class ItemCheck extends React.Component {
  constructor(props) {
    super(props)
  }

  render() {
    return (
      <FormControl component="fieldset">
        <FormGroup aria-label="position" row>
          <FormControlLabel
            value="start"
            control={<Switch color="primary" />}
            label={this.props.label}
            labelPlacement="start"
          />
        </FormGroup>
      </FormControl>
    );
  }
}

ReactDOM.render(<ItemCheck label="start label" />, document.getElementById("root"));