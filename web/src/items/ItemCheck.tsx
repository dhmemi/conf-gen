import * as React from 'react';
import Switch from '@mui/material/Switch';
import FormGroup from '@mui/material/FormGroup';
import FormControlLabel from '@mui/material/FormControlLabel';
import FormControl from '@mui/material/FormControl';
import FormLabel from '@mui/material/FormLabel';

interface CheckProp {
    onClick: Function,
    data: {
        comment: string,
        dtype: string,
        index: number,
        mtype: number,
        name: string,
        premission: number,
        value: boolean
    }
}

export default class ItemCheck extends React.Component<CheckProp> {
    constructor(props: CheckProp) {
        super(props)
    }
    render() {
        return (
            <FormControl component="fieldset">
                <FormGroup aria-label="position" row>
                    <FormControlLabel
                        value="start"
                        control={<Switch color="primary" checked={this.props.data.value} onClick={() => this.props.onClick()} />}
                        label={this.props.data.name + ":"}
                        labelPlacement="start"
                    />
                </FormGroup>
            </FormControl>
        );
    }
}