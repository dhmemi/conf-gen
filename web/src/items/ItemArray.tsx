import * as React from 'react';

import FormGroup from '@mui/material/FormGroup';
import FormControlLabel from '@mui/material/FormControlLabel';
import FormControl from '@mui/material/FormControl';
import FormLabel from '@mui/material/FormLabel';

interface ArrayProp {
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

export default class ItemArray extends React.Component<ArrayProp> {
    constructor(props: ArrayProp) {
        super(props)
    }
    render() {
        return (
            <FormControl component="fieldset">
                <FormGroup aria-label="position" row>
                </FormGroup>
            </FormControl>
        );
    }
}