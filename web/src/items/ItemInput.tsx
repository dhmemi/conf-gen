import * as React from 'react';

import FormGroup from '@mui/material/FormGroup';
import FormControlLabel from '@mui/material/FormControlLabel';
import FormControl from '@mui/material/FormControl';
import FormLabel from '@mui/material/FormLabel';
import { TextField } from '@mui/material';

interface InputProp {
    onClick: Function,
    data: {
        comment: string,
        dtype: string,
        index: number,
        mtype: number,
        name: string,
        premission: number,
        value: number,
        ctrls: {
            ge: number,
            le: number,
            step: number
        }
    }
}

export default class ItemInput extends React.Component<InputProp> {
    constructor(props: InputProp) {
        super(props)
    }
    render() {
        return (
            <FormControl component="fieldset">
                <FormGroup aria-label="position" row>
                    <TextField
                        id="outlined-number"
                        type="number"
                        label={this.props.data.name}
                        InputLabelProps={{
                            shrink: true,
                        }}
                        variant="outlined"
                        size="small"
                    />
                </FormGroup>
            </FormControl>
        );
    }
}
